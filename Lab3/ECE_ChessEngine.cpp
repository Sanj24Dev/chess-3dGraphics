/**
 * @file ECE_ChessEngine.cpp
 * @author Sanjana Ganesh Nayak
 * @brief 
 * @version 0.1
 * @date 2024-11-26
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <iostream>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sstream>

/**
 * @brief Class for handling chess engine
 * 
 */
class ECE_ChessEngine {
private:
    int parent_to_child[2];
    int child_to_parent[2];
    pid_t pid;

public:
    // Initialize pipes
    ECE_ChessEngine() {
        pipe(parent_to_child);
        pipe(child_to_parent);
    }

    // Destructor
    ~ECE_ChessEngine() {
        close(parent_to_child[1]);
        close(child_to_parent[0]);

        // Wait for the child process to finish
        if (pid != 0) {
            wait(nullptr); 
        }
    }

    /**
     * @brief Set the Options in Komodo
     * 
     * @param option 
     * @return bool status 
     */
    bool setOptions(std::string option) {
        if (pid != 0)
        {
            std::string command = "setoption name " + option + "\n";
            if (write(parent_to_child[1], command.c_str(), command.size()) == -1) {
                std::cerr << "Error writing to pipe" << std::endl;
                return false;
            }
            std::cout << "Komodo Chess Engine: " << option << "\n";
            return true;
        }
    }

    /**
     * @brief Initialize the Komodo Engine
     * 
     * @return bool status 
     */
    bool InitializeEngine() {
        pid = fork();
        // Set up the pipes and start the komodo engine
        if (pid == 0) {
            close(parent_to_child[1]);
            dup2(parent_to_child[0], STDIN_FILENO);
            close(parent_to_child[0]);

            close(child_to_parent[0]);
            dup2(child_to_parent[1], STDOUT_FILENO);
            close(child_to_parent[1]);

            // Launch the Komodo chess engine
            if (execlp("/home/hice1/snayak89/labs/final_project/build/komodo-14.1-linux", "komodo", nullptr) == -1) {
                std::cerr << "Failed to start Komodo. Make sure Komodo is in the current directory.\n";
                exit(EXIT_FAILURE);
            }
        } else {
            close(parent_to_child[0]);
            close(child_to_parent[1]);

            std::string command = "uci\n";
            if (write(parent_to_child[1], command.c_str(), command.size()) == -1) {
                std::cerr << "Error writing to pipe" << std::endl;
                return false;
            }
            std::cout << "Komodo Chess Engine Initialized.\n";
        }
        return true;
    }

    /**
     * @brief Send the user's move to the Komodo engine
     * 
     * @param moveHistory 
     * @return bool status 
     */
    bool sendMove(std::string moveHistory) {
        if (pid != 0)
        {
            std::string command = "position startpos moves " + moveHistory + "\n";
            if (write(parent_to_child[1], command.c_str(), command.size()) == -1) {
                std::cerr << "Error writing to pipe" << std::endl;
                return false;
            }
            command = "go\n";
            if (write(parent_to_child[1], command.c_str(), command.size()) == -1) {
                std::cerr << "Error writing to pipe" << std::endl;
                return false;
            }
            return true;
        }
    }


    /**
     * @brief Get the Response Move from the Komodo Engine
     * 
     * @param strMove 
     * @return bool status 
     */
    bool getResponseMove(std::string& strMove) {
        if (pid != 0)
        {
            char buffer[256];
            int bytesRead;
            std::string komodoOutput;

            while ((bytesRead = read(child_to_parent[0], buffer, sizeof(buffer) - 1)) > 0) {
                if (bytesRead >= sizeof(buffer)) {
                    std::cerr << "Buffer overflow detected." << std::endl;
                    return false;
                }
                buffer[bytesRead] = '\0';
                komodoOutput += buffer;

                std::size_t found = komodoOutput.find("bestmove");
                std::string word, move;
                if (found != std::string::npos) {
                    std::istringstream iss(komodoOutput.substr(found));
                    iss >> word; 
                    iss >> move; 
                    std::cout << "Komodo Move: " << move << std::endl;
                    strMove = move; // Return the best move found
                    return true;
                }
            }
            return false;
        }
    }
};
