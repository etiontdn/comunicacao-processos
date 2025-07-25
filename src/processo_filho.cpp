#include <iostream>
#include <string>
#include <unistd.h> // For getpid(), read(), write(), close()

int main(int argc, char *argv[]) {
    // Expecting the pipe write file descriptor as an argument
    if (argc < 2) {
        std::cerr << "Child: Error: Missing pipe write FD argument." << std::endl;
        return 1;
    }

    int write_fd_to_parent = std::stoi(argv[1]); // Convert string argument to integer FD

    std::string user_input;
    std::cout << "\n--- Child Process " << getpid() << " ---" << std::endl;
    std::cout << "Child " << getpid() << ": Please enter your message: ";
    std::getline(std::cin, user_input); // Read a line of user input

    std::string response_message = "Child " + std::to_string(getpid()) + " says: '" + user_input + "'\n";
    
    if (write(write_fd_to_parent, response_message.c_str(), response_message.length()) == -1) {
        perror("child: write to parent failed");
    }

    close(write_fd_to_parent); // Close the write end when done
    std::cout << "Child " << getpid() << ": Message sent, exiting." << std::endl;
    return 0; // Child exits
}