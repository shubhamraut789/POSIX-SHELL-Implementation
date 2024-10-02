#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <vector>
#include <string.h>
#include "functions.h"

using namespace std;

string call_re_echo(string& str) {
    string s = "";
    bool flag = false;
    bool started = false; 
    long i = 0;

    while (i < str.size() && isspace(str[i])) {
        i++;
    }

    // Skip the "echo" command 
    string command = "echo";
    if (str.substr(i, command.size()) == command) {
        i += command.size();
    }else i=i;

    while (i < str.size() && isspace(str[i])) {
        i++;
    }

    for (; i < str.size(); i++) {
        if (str[i] == '"') {
            flag = !flag;  // Toggle the flag when encountering a quote
            started = true;
            continue;
        }

        if(!flag && isspace(str[i])){
            if(started && !s.empty() && s.back() != ' '){
                s += ' ';
            }
        }else{
            s += str[i];
            started = true;
        }
    }

    if (!s.empty() && s.back() == ' ') {
        s.pop_back();
    }

    //cout << s << '\n';
    return s;
}

void handle_redirection(vector<string>& tokens) {
    int ip_fd = -1, out_fd = -1;
    bool ip_flag = false, out_flag = false, append_flag = false;
    vector<string> command;
    
    for (size_t i = 0; i < tokens.size(); ++i) {
        if (tokens[i] == "<") {
            if (i + 1 < tokens.size()) {
                string iat1 = tokens[i+1];
                char iapt1[iat1.size()+1];

                for(int i=0; i<iat1.size(); i++){
                    iapt1[i] = iat1[i];
                }

                iapt1[iat1.size()] = '\0';
                ip_fd = open(tokens[i + 1].c_str(), O_RDONLY);
                if (ip_fd < 0) {
                    cout<<"Error while opening input file"<<'\n';
                    return;
                }
                ip_flag = true;
                i++; 
            } else {
                cout<< "bash: syntax error near unexpected token `newline'" <<'\n';
                return;
            }
        } else if (tokens[i] == ">") {
            if (i + 1 < tokens.size()) {
                
                string at1 = tokens[i+1];
                char apt1[at1.size()+1];

                for(int i=0; i<at1.size(); i++){
                    apt1[i] = at1[i];
                }

                apt1[at1.size()] = '\0';
                out_fd = open(apt1, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (out_fd < 0) {
                    cout<<"Error while opening/creating output file"<<'\n';
                    return;
                }
                out_flag = true;
                i++; 
            } else {
                cout<< "bash: syntax error near unexpected token `newline'" <<'\n';
                return;
            }
        } else if (tokens[i] == ">>") {
            if (i + 1 < tokens.size()) {
                string t1 = tokens[i+1];
                char ct1[t1.size()+1];

                for(int i=0; i<t1.size(); i++){
                    ct1[i] = t1[i];
                }

                ct1[t1.size()] = '\0';
                out_fd = open(ct1, O_WRONLY | O_CREAT | O_APPEND, 0644);
                if (out_fd < 0) {
                    cout<<"Error opening/creating output file"<<'\n';
                    return;
                }
                out_flag = true;
                append_flag = true;
                i++; 
            } else {
                cout<< "bash: syntax error near unexpected token `newline'" <<'\n';
                return;
            }
        } else {
            command.push_back(tokens[i]);
        }
    }

    if (!command.empty() && command[0] == "echo") {

        string echo_str;
        for (size_t i = 1; i < command.size(); ++i) {
            echo_str += command[i] + " ";
        }

        string processed_echo = call_re_echo(echo_str);
        
        char echo_s[processed_echo.size()+1];

        for(int i=0; i<processed_echo.size(); i++){
            echo_s[i] = processed_echo[i];
        }

        echo_s[processed_echo.size()] = '\0';

        if(out_flag){
            if(out_fd != -1){
                write(out_fd, echo_s, processed_echo.length());
                write(out_fd, "\n", 1);
            }
        }else cout << echo_str <<'\n';
        return;
    }

    pid_t pid = fork();
    if (pid == 0) {
        if (ip_flag) {
            dup2(ip_fd, STDIN_FILENO);
        }
        if (out_flag) {
            dup2(out_fd, STDOUT_FILENO);
        }

        vector<char*> excargs;
        
        for(size_t i = 0; i < command.size(); ++i){
            excargs.push_back((char*)command[i].c_str());
        }
        excargs.push_back(nullptr);

        execvp(excargs[0], excargs.data());
        cout<<"execvp failed to execute"<<'\n';

    }else if(pid > 0) wait(NULL);

    else cout<<"fork failed to create child process"<<'\n';
}

