#include <iostream>
#include <string.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <cstdio>
#include <vector>
#include <fstream>
#include <fcntl.h>
#include <sstream>
#include <cstdlib>
#include "functions.h"

using namespace std;

void call_pinfo(int cnt, vector<string> args)
{
    if (cnt == 1)
    {
        int pid = getpid();
        cout << "Process ID: " << pid << endl;

        stringstream status_path;
        status_path << "/proc/" << pid << "/stat";

        ifstream status_file(status_path.str());
        if (status_file.is_open())
        {
            string status_line;
            getline(status_file, status_line);
            status_file.close();

            stringstream ss(status_line);
            string token;
            int token_count = 1;
            while (getline(ss, token, ' '))
            {
                if (token_count == 3)
                {
                    cout << "Status: " << token << endl;
                }
                else if (token_count == 23)
                {
                    cout << "Memory Usage: " << stol(token) / 1024 << " KB" << endl;
                }
                token_count++;
            }
        }

        stringstream exe_path;
        exe_path << "/proc/" << pid << "/exe";
        char executable_path[128];
        ssize_t path_length = readlink(exe_path.str().c_str(), executable_path, sizeof(executable_path));
        if (path_length != -1)
        {
            executable_path[path_length] = '\0';
            cout << "Executable Path: " << executable_path << endl;
        }
        else
        {
            cout << "Error reading executable path: " << strerror(errno) << endl;
        }
    }
    else if (cnt == 2)
    {
        int pid = (stoi(args[1]));
        if (kill(pid, 0) == 0)
        {
            cout << "Process ID: " << pid << endl;
            stringstream status_path;
            status_path << "/proc/" << pid << "/stat";

            ifstream status_file(status_path.str());
            if (status_file.is_open())
            {
                string status_line;
                getline(status_file, status_line);
                status_file.close();

                stringstream ss(status_line);
                string token;
                int token_count = 1;
                while (getline(ss, token, ' '))
                {
                    if (token_count == 3)
                    {
                        cout << "Status: " << token << endl;
                    }
                    else if (token_count == 23)
                    {
                        cout << "Memory Usage: " << stol(token) / 1024 << " KB" << endl;
                    }
                    token_count++;
                }
            }

            stringstream exe_path;
            exe_path << "/proc/" << pid << "/exe";
            char executable_path[128];
            ssize_t path_length = readlink(exe_path.str().c_str(), executable_path, sizeof(executable_path));
            if (path_length != -1)
            {
                executable_path[path_length] = '\0';
                cout << "Executable Path: " << executable_path << endl;
            }
            else
            {
                cout << "Error reading executable path: " << strerror(errno) << endl;
            }
        }
        else
        {
            cout << "Invalid process ID: " << pid << endl;
        }
    }
    else
    {
        cerr << "Usage: pinfo [PID]" << endl;
    }
}