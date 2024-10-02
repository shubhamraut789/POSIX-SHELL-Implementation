#include <iostream>
#include <unistd.h>
#include <string>
#include <vector>
#include "functions.h"

using namespace std;

// Variables to store current and previous directories
string curr_dir = "";
string prev_dir = "";
int cd_dash_cnt = 0;

void update_directories() {
    char temp[128];
    if (getcwd(temp, sizeof(temp)) != nullptr) {
        prev_dir = curr_dir;
        curr_dir = temp;
    } else {
        cout<<"Error getting current directory"<<'\n';
    }
}

void initialize_directories(){
    char temp[128];
    if (getcwd(temp, sizeof(temp)) != nullptr) {
        curr_dir = temp;
    } else {
        cout<<"Error getting current directory during initialization"<<'\n';
    }
}

void call_cd(const vector<string>& tokens) {

    if(curr_dir == "") initialize_directories();

    if(tokens.size() == 2){
        if(tokens[1] == "-"){
            cd_dash_cnt++;
            if(!prev_dir.empty()){
                if(chdir(prev_dir.c_str()) == 0){
                    update_directories();
                }else{
                    cout<<"Error changing to previous directory"<<'\n';
                }
            }else{
                cout << "No previous directory to switch to." <<'\n';
            }
        }else if(tokens[1] == "~"){
            const char* home = getenv("HOME");
            if(home == nullptr){
                cout << "Error: HOME environment variable is not set." <<'\n';
            }else{
                if(chdir(home) == 0){
                    update_directories();
                }else{
                    cout<<"Error changing to home directory"<<'\n';
                }
            }
        }else{
            if(chdir(tokens[1].c_str()) == 0){
                update_directories();
            }else{
                cout<<"Error changing directory"<<'\n';
            }
        }
    }else{
        const char* home = getenv("HOME");
        if(home == nullptr){
            cout << "Error: HOME environment variable is not set." <<'\n';
        }else{
            if(chdir(home) == 0){
                update_directories();
            }else{
                cout<<"Error changing to home directory"<<'\n';
            }
        }
    }
}
