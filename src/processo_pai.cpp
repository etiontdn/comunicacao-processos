#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>     // For fork(), pipe(), read(), write(), close(), getpid(), execlp()
#include <sys/wait.h>   // For waitpid()
#include <algorithm>    // For std::remove_if (useful for cleaning up defunct PIDs)
#include <cstdio>       // For perror

// Determine which terminal emulator to use
// You might need to change this based on your system
#define TERMINAL_EMULATOR "xterm" // e.g., "xterm", "konsole", "kitty", "gnome-terminal"
#define TERMINAL_ARG "-e"         // e.g., "-e" for xterm, "--execute" for konsole/kitty, "--" for gnome-terminal

int main() {
    std::cout << "Parent process (PID: " << getpid() << ") started." << std::endl;

    std::vector<pid_t> child_pids;
    std::vector<int> parent_read_fds; // Store the read FDs for the parent for each child

    // Get the current executable's directory to find the child program
    char self_path_buffer[1024];
    ssize_t count = readlink("/proc/self/exe", self_path_buffer, sizeof(self_path_buffer) - 1);
    if (count == -1) {
        perror("readlink /proc/self/exe failed");
        return 1;
    }
    self_path_buffer[count] = '\0';
    std::string parent_executable_path = self_path_buffer;
    size_t last_slash_pos = parent_executable_path.rfind('/');
    std::string executable_dir = (last_slash_pos != std::string::npos) ?
                                  parent_executable_path.substr(0, last_slash_pos + 1) :
                                  "./";
    
    std::string child_executable_path = executable_dir + "processo_filho"; // Name of the child executable

    std::cout << "\nParent: Enter '1' to create a new child process, or 'q' to quit." << std::endl;
    std::string user_input;
    int child_counter = 0; // To keep track of how many children are spawned

    while (true) {
        std::cout << "\nParent> ";
        std::cin >> user_input;

        if (user_input == "1") {
            child_counter++;
            int pipe_child_to_parent_fds[2]; // Pipe: Child writes, Parent reads

            if (pipe(pipe_child_to_parent_fds) == -1) {
                perror("pipe failed");
                break; // Exit loop on critical error
            }

            pid_t pid = fork();

            if (pid == -1) {
                perror("fork failed");
                close(pipe_child_to_parent_fds[0]);
                close(pipe_child_to_parent_fds[1]);
                break; // Exit loop on critical error
            } else if (pid == 0) { // Child process
                // Close the read end of the pipe in the child (it only writes)
                close(pipe_child_to_parent_fds[0]);

                // Convert the write FD to a string to pass as argument
                std::string write_fd_str = std::to_string(pipe_child_to_parent_fds[1]);

                // Exec a new terminal window to run the child_exec_program
                execlp(TERMINAL_EMULATOR, TERMINAL_EMULATOR, TERMINAL_ARG,
                       child_executable_path.c_str(), // Path to your actual child program
                       write_fd_str.c_str(),          // Pass the write FD as a string argument
                       NULL); // MUST be NULL-terminated

                // If execlp returns, it means an error occurred
                perror("execlp failed");
                exit(1); // Child must exit on exec failure
            } else { // Parent process
                // Close the write end of the pipe in the parent (it only reads)
                close(pipe_child_to_parent_fds[1]); 
                child_pids.push_back(pid);
                parent_read_fds.push_back(pipe_child_to_parent_fds[0]); // Store the read FD for this child
                std::cout << "Parent: Forked child process (PID: " << pid << ") in a new terminal. Total children: " << child_counter << std::endl;
            }
        } else if (user_input == "q") {
            std::cout << "Parent: Quitting. Waiting for all active children to terminate..." << std::endl;
            break; // Exit the loop
        } else {
            std::cout << "Parent: Invalid input. Please enter '1' to create a child or 'q' to quit." << std::endl;
        }

        // --- Parent's continuous monitoring for child messages ---
        // This part runs in the main loop of the parent,
        // so it can receive messages from children even while waiting for new input.
        
        // Use a temporary vector to store FDs that are still open
        std::vector<int> active_read_fds;
        std::vector<pid_t> active_child_pids;

        for (size_t i = 0; i < parent_read_fds.size(); ++i) {
            int read_fd = parent_read_fds[i];
            pid_t child_pid = child_pids[i];

            // Check if there's data in the pipe without blocking
            fd_set fds;
            struct timeval timeout;

            FD_ZERO(&fds);
            FD_SET(read_fd, &fds);

            timeout.tv_sec = 0; // Don't wait
            timeout.tv_usec = 10000; // 10ms timeout

            int ready = select(read_fd + 1, &fds, NULL, NULL, &timeout);

            if (ready > 0) {
                char buffer[256];
                std::string child_response_buffer;
                ssize_t bytes_read;

                // Read all available data, or until newline
                while ((bytes_read = read(read_fd, buffer, sizeof(buffer) - 1)) > 0) {
                    buffer[bytes_read] = '\0';
                    child_response_buffer += buffer;
                    if (child_response_buffer.find('\n') != std::string::npos) {
                        break;
                    }
                }

                if (bytes_read == -1) {
                    perror("parent: read from child failed during monitoring");
                    close(read_fd); // Close bad FD
                } else if (bytes_read == 0 && child_response_buffer.empty()) {
                    // EOF received and no data. Child has likely closed its write end and exited.
                    std::cout << "Parent: Child " << child_pid << " pipe closed (EOF received)." << std::endl;
                    close(read_fd); // Close this FD as the child is done
                } else {
                    size_t newline_pos = child_response_buffer.find('\n');
                    if (newline_pos != std::string::npos) {
                        child_response_buffer.erase(newline_pos + 1);
                    }
                    std::cout << "Parent received from child " << child_pid << ": " << child_response_buffer;
                    // If still more data expected from this child, keep its FD open
                    active_read_fds.push_back(read_fd);
                    active_child_pids.push_back(child_pid);
                }
            } else if (ready == -1) {
                perror("select failed");
                close(read_fd); // Close bad FD
            } else { // ready == 0, no data, pipe still open
                active_read_fds.push_back(read_fd);
                active_child_pids.push_back(child_pid);
            }
        }
        // Update the lists of active FDs and PIDs for the next iteration
        parent_read_fds = active_read_fds;
        child_pids = active_child_pids;

        // Also reap any zombie children periodically (non-blocking waitpid)
        int status;
        pid_t terminated_pid;
        while ((terminated_pid = waitpid(-1, &status, WNOHANG)) > 0) {
            std::cout << "Parent: Reaped terminated child (terminal) " << terminated_pid << " with status " << status << std::endl;
        }
    }

    // Final cleanup: Close any remaining open pipe FDs
    for (int fd : parent_read_fds) {
        close(fd);
    }
    
    // Final wait for all children that might still be running (e.g., if user quits before they're done)
    std::cout << "Parent: Ensuring all remaining children are reaped..." << std::endl;
    while (wait(NULL) > 0); 

    std::cout << "Parent process finished." << std::endl;

    return 0;
}