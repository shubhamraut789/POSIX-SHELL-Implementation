#include <iostream>
#include <unistd.h>
#include <syscall.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>
#include <algorithm>
#include <map>
#include <sys/utsname.h>
#include <errno.h>
#include <vector>
#include <termios.h>
#include "functions.h"

using namespace std;

vector<string> tokenize_ip(const string& ip){
    vector<string> tokens;
    char inputcpy[ip.length()+1];

    for(long long i=0; i<ip.size(); i++){
        inputcpy[i] = ip[i];
    }
    inputcpy[ip.length()] ='\0';

    char delimator[] = " ";
    char* token = strtok(inputcpy,delimator);

    while(token != nullptr){
        tokens.push_back(string(token));
        token = strtok(nullptr,delimator);
    }

    return tokens;
}


int main(){

    string ip_command;

    while(true){

        displayPrompt();

        getline(cin,ip_command);
        string filename = "history.txt";
        
        vector<string> tokens = tokenize_ip(ip_command);

        if (!tokens.empty()) {

            add_to_history(ip_command, filename);

            if (tokens[0] == "exit") break;

            if (find(tokens.begin(), tokens.end(), "|") != tokens.end()) {
                vector<string> commands;
                string current_command;
                for (const string& token : tokens) {
                    if (token == "|") {
                        commands.push_back(current_command);
                        current_command.clear();
                    } else {
                        if (!current_command.empty()) current_command += " ";
                        current_command += token;
                    }
                }
                if (!current_command.empty()) commands.push_back(current_command);

                call_pipes(commands);
            }
            else if(tokens[0] == "pinfo") call_pinfo(tokens.size(),tokens);
            else if(tokens[0] == "pwd") call_pwd();
            else if(find(tokens.begin(), tokens.end(), "<") != tokens.end() || 
                find(tokens.begin(), tokens.end(), ">") != tokens.end() ||
                find(tokens.begin(), tokens.end(), ">>") != tokens.end()){
                handle_redirection(tokens);
            }

           else if(tokens[0] == "echo"){
                if(tokens[1] != ""){
                    call_echo(ip_command);
                }
            }

            else if(tokens.size() == 3 && tokens[1]=="-a" && tokens[2] == "-l") call_ls_al();
            else if(tokens.size() > 1 && tokens[0] == "ls"){
                if(tokens[1] == "-al" || tokens[1] == "-la") call_ls_al();
                else if(tokens[1] == "..") call_ls_parent();
                else if(tokens[1] == "-a") call_ls_a();
                else if(tokens[1] == "-l") call_ls_l();
                else if(tokens[1] == "~" || tokens[1] == ".") call_ls();                
            }
            
            else if(tokens.size() == 1 && tokens[0] == "ls"){
                call_ls();
            }
            else if(tokens[0] == "cd") call_cd(tokens);
            else if(tokens[0] == "search") call_search(tokens[1]);
            else if(tokens[0] == "history"){
                if(tokens.size() == 2){
                    int num = stoi(tokens[1]);
                    call_history1(num);
                }
                else if(tokens.size() == 1) call_history();
                else cout<<"bash: history: too many arguments"<<'\n';
            }
            else{
                cout<<tokens[0]<<": command not found"<<'\n';
            }
        }
    }

    return 0;
}