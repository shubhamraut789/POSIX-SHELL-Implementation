#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstring>
#include <iostream>
#include <algorithm>
#include "functions.h"

using namespace std;

void call_pipes(const vector<string>& commands) {
    int num_pipes = commands.size() - 1;
    vector<int[2]> pipes(num_pipes);

    // Create pipes
    for (int i = 0; i < num_pipes; ++i) {
        if (pipe(pipes[i]) == -1) {
            cout<<"pipe success"<<'\n';
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < commands.size(); ++i) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // Child process

            if (i > 0) { // Not the first command
                dup2(pipes[i - 1][0], STDIN_FILENO); // Set stdin to read end of previous pipe
            }

            if (i < num_pipes) { // Not the last command
                dup2(pipes[i][1], STDOUT_FILENO); // Set stdout to write end of current pipe
            }

            // Close all pipe file descriptors
            for (int j = 0; j < num_pipes; ++j) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }

            // Convert command to char* array
            vector<char*> args;
            vector<string> tokens = tokenize_ip(commands[i]);
            for (const string& token : tokens) {
                args.push_back(const_cast<char*>(token.c_str()));
            }
            args.push_back(nullptr);

            // Execute the command
            execvp(args[0], args.data());
            perror("execvp");
            exit(EXIT_FAILURE);
        }
    }

    // Parent process closes all pipe file descriptors
    for (int i = 0; i < num_pipes; ++i) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    // Wait for all child processes
    for (int i = 0; i < commands.size(); ++i) {
        wait(nullptr);
    }
}
