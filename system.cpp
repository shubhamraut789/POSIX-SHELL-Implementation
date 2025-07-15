#include <iostream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>

using namespace std;

void execute_system_command(vector<string> tokens, bool background) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return;
    }

    if (pid == 0) {
        // Child process

        // Convert vector<string> to char* array for execvp
        vector<char*> args;
        for (string& tok : tokens) {
            args.push_back((char*)tok.c_str());
        }
        args.push_back(NULL);

        if (execvp(args[0], args.data()) < 0) {
            perror("execvp failed");
            exit(EXIT_FAILURE);
        }
    } else {
        // Parent process
        if (background) {
            cout << pid << endl;  // print background process PID
        } else {
            waitpid(pid, NULL, 0);  // wait for foreground process
        }
    }
}
