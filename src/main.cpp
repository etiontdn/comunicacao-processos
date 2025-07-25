#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>     // For fork(), pipe(), read(), write(), close()
#include <sys/wait.h>   // For wait()
#include <algorithm>    // For std::find_if

// Function to simulate child process work
void child_process_work(int write_fd_to_parent, int child_num) {
    // Child closes its unused pipe end (the read end of the child_to_parent pipe)
    // Assuming write_fd_to_parent is pipefd[1], then pipefd[0] is the read end.
    close(write_fd_to_parent - 1); 

    std::string user_input;
    std::cout << "\n--- Child " << getpid() << " (Message #" << child_num << ") ---" << std::endl;
    std::cout << "Child " << getpid() << ": Please enter your message: ";
    std::getline(std::cin, user_input); // Read a line of user input

    std::string response_message = "Child " + std::to_string(getpid()) + " says: '" + user_input + "' (Message #" + std::to_string(child_num) + ")\n";
    
    if (write(write_fd_to_parent, response_message.c_str(), response_message.length()) == -1) {
        perror("child: write to parent failed");
    }

    close(write_fd_to_parent); // Close the write end when done
    exit(0); // Child exits
}

int main() {
    std::cout << "Parent process (PID: " << getpid() << ") started." << std::endl;

    const int NUM_CHILDREN = 3;
    std::vector<pid_t> child_pids;
    // We only need one pipe per child: child writes to parent
    std::vector<std::pair<int, int>> child_to_parent_pipes(NUM_CHILDREN); // {read_fd, write_fd} for parent's perspective

    for (int i = 0; i < NUM_CHILDREN; ++i) {
        int pipe_child_to_parent_fds[2]; // Pipe: Child writes, Parent reads

        if (pipe(pipe_child_to_parent_fds) == -1) {
            perror("pipe failed");
            // Clean up already created pipes and wait for children
            return 1;
        }

        pid_t pid = fork();

        if (pid == -1) {
            perror("fork failed");
            // Close pipes and cleanup existing children
            return 1;
        } else if (pid == 0) { // Child process
            // Close parent's ends of pipes in child
            close(pipe_child_to_parent_fds[0]); // Child closes read end (it only writes)
            child_process_work(pipe_child_to_parent_fds[1], i + 1);
            // child_process_work will exit, so no code below this point for child
        } else { // Parent process
            // Close child's ends of pipes in parent
            close(pipe_child_to_parent_fds[1]); // Parent closes write end (it only reads)

            child_pids.push_back(pid);
            child_to_parent_pipes[i] = {pipe_child_to_parent_fds[0], pipe_child_to_parent_fds[1]}; // Storing {read_fd, write_fd} for parent (read_fd is for parent to read from child)

            std::cout << "Parent: Forked child " << pid << " for message #" << i + 1 << std::endl;
        }
    }

    std::cout << "\nParent: All child processes forked. Waiting for child messages..." << std::endl;

    // Parent reads responses from each child
    // This loop structure assumes a sequential reading.
    // In a real-world concurrent scenario, you might use select/poll or
    // multiple threads in the parent to read from pipes simultaneously.
    for (int i = 0; i < NUM_CHILDREN; ++i) {
        char buffer[256];
        ssize_t bytes_read;
        std::string child_response_buffer;

        std::cout << "Parent: Reading from child " << child_pids[i] << "'s pipe..." << std::endl;
        
        // Loop to ensure entire message is read if it exceeds buffer size
        while ((bytes_read = read(child_to_parent_pipes[i].first, buffer, sizeof(buffer) - 1)) > 0) {
            buffer[bytes_read] = '\0'; // Null-terminate
            child_response_buffer += buffer;
            // Check for newline delimiter, if present, process the message and break
            if (child_response_buffer.find('\n') != std::string::npos) {
                break; 
            }
        }

        if (bytes_read == -1) {
            perror("parent: read from child failed");
        } else if (bytes_read == 0 && child_response_buffer.empty()) {
            std::cout << "Parent: Child " << child_pids[i] << " closed its pipe without sending a message." << std::endl;
        } else {
            // Trim potential extra characters after newline if full_message_buffer contains more
            size_t newline_pos = child_response_buffer.find('\n');
            if (newline_pos != std::string::npos) {
                child_response_buffer.erase(newline_pos + 1); // Keep the newline for consistency
            }
            std::cout << "Parent received from child " << child_pids[i] << ": " << child_response_buffer;
        }
        close(child_to_parent_pipes[i].first); // Close parent's read end for this child
    }

    // Wait for all child processes to terminate
    for (pid_t child_pid : child_pids) {
        int status;
        waitpid(child_pid, &status, 0);
        std::cout << "Parent: Child " << child_pid << " terminated with status " << status << std::endl;
    }

    std::cout << "Parent process finished. All interactions complete." << std::endl;

    return 0;
}