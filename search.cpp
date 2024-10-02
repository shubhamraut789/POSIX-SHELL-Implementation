#include <iostream>
#include <unistd.h>
#include <syscall.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <algorithm>
#include <map>
#include <sys/utsname.h>
#include <errno.h>
#include <dirent.h>
#include "functions.h"
#include <vector>

using namespace std;

void search(char* path,string& str,bool& flag){
    DIR* dir;
    struct dirent* entry;

    dir = opendir(path);
    if(dir == nullptr){
        perror("Error opening directory");
    }

    while((entry = readdir(dir)) != NULL){

        // if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
        //     continue;
        // }
        
        if (entry->d_name[0] == '.' && (entry->d_name[1] == '\0' || (entry->d_name[1] == '.' && entry->d_name[2] == '\0'))) {
            continue;
        }

        string full_path = string(path) + "/" + entry->d_name;

        if(str == entry->d_name){
            flag = true;
            closedir(dir);
            return;
        }

        char path_data[full_path.size()+1];

        for(int i=0; i<full_path.size(); i++){
            path_data[i] = full_path[i];
        }
        
        path_data[full_path.size()] = '\0';

        if(entry->d_type == DT_DIR){
            search(path_data, str, flag); 
            if(flag){  
                closedir(dir);
                return;
            }
        }
    }

    closedir(dir);
}

void call_search(string& str){

    char current_directory[128];
    char* path = getcwd(current_directory, sizeof(current_directory));

    bool flag = false;
    search(path, str, flag);

    if(flag) cout << "True" << '\n';
    else cout << "False" << '\n';

}