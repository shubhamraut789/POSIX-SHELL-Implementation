#include <iostream>
#include <unistd.h>
#include <sys/utsname.h>
#include <string>
#include "functions.h"

using namespace std;

void displayPrompt(){
    char* username = getlogin();
    
    char* sys_name;
    struct utsname uts;
    uname(&uts);

    if(uname(&uts) < 0){
        cout<<"error while defining struct for system"<<'\n';
    }else sys_name = uts.nodename;

    char curr_directory[1024];
    char* status = getcwd(curr_directory, sizeof(curr_directory));

    if(!status){
        cout<<"Error while retrieving current directory"<<'\n';
        return;
    }

    // Retrieve the home directory path
    const char* home_dir = getenv("HOME");
    string display_dir = curr_directory;

    if (home_dir != nullptr) {
        string home = home_dir;

        // Check if current directory is home or a subdirectory of home
        if (display_dir == home) {
            display_dir = "~";
        } else if (display_dir.find(home) == 0) {
            display_dir = "~" + display_dir.substr(home.length());
        }
    }

    cout << username << "@" << sys_name << ":" << display_dir << "> ";
    
}
