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
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include "functions.h"

using namespace std;

void ls_a(char* path){

    DIR* dir;
    struct dirent* entry;
    
    dir = opendir(path);
    if (dir == nullptr) {
        perror("Error opening directory: ");
        return;
    }
    
    while ((entry = readdir(dir)) != NULL) {
        cout << entry->d_name <<'\n';
    }

}

void call_ls_a(){

    char current_directory[128];
    char* path = getcwd(current_directory,sizeof(current_directory));
    ls_a(path);

}

void ls(const char* path, bool show_hidden = false){

    DIR* dir;
    struct dirent* entry;
    
    dir = opendir(path);
    if (dir == nullptr) {
        perror("Error opening directory: ");
        return;
    }
    
    while ((entry = readdir(dir)) != NULL) {
        if (!show_hidden && entry->d_name[0] == '.') {
            continue; 
        }
        cout << entry->d_name <<'\n';
    }

}

void call_ls(){

    bool show_hidden = false;
    char current_directory[128];
    char* path = getcwd(current_directory, sizeof(current_directory));
    ls(path, show_hidden);

}



void print_permissions(mode_t mode) {

    if(S_ISDIR(mode)) cout<< 'd'; else cout<< '-';
    if((mode & S_IRUSR)) cout<< 'r'; else cout<< '-';
    if((mode & S_IWUSR)) cout<< 'w'; else cout<< '-';
    if((mode & S_IXUSR)) cout<< 'x'; else cout<< '-';
    if((mode & S_IRGRP)) cout<< 'r'; else cout<< '-';
    if((mode & S_IWGRP)) cout<< 'w'; else cout<< '-';
    if((mode & S_IXGRP)) cout<< 'x'; else cout<< '-';
    if((mode & S_IROTH)) cout<< 'r'; else cout<< '-';
    if((mode & S_IWOTH)) cout<< 'w'; else cout<< '-';
    if((mode & S_IXOTH)) cout<< 'x'; else cout<< '-';

}

void print_file_info(char* path,char* name) {
    struct stat file_stat;
    string full_path = string(path) + "/" + name;

    if (stat(full_path.c_str(), &file_stat) == -1) {
        perror("Error getting file stats");
        return;
    }

    print_permissions(file_stat.st_mode);
    cout << ' ' << file_stat.st_nlink;
    
    struct passwd* pwd = getpwuid(file_stat.st_uid);
    struct group* grp = getgrgid(file_stat.st_gid);
    cout << ' ' << (pwd ? pwd->pw_name : to_string(file_stat.st_uid).c_str())
         << ' ' << (grp ? grp->gr_name : to_string(file_stat.st_gid).c_str())
         << ' ' << file_stat.st_size;
    
    // Time formatting
    char time_buff[64];
    struct tm* timeinfo = localtime(&file_stat.st_mtime);
    strftime(time_buff, sizeof(time_buff), "%b %d %H:%M", timeinfo);
    cout << ' ' << time_buff << ' ' << name << '\n';
}

void ls_al(char* path) {
    DIR* dir;
    struct dirent* entry;

    dir = opendir(path);
    if (dir == nullptr) {
        perror("Error opening directory");
        return;
    }

    while ((entry = readdir(dir)) != nullptr) {
        print_file_info(path, entry->d_name);
    }

}

void call_ls_al() {

    char current_directory[128];
    char* path = getcwd(current_directory, sizeof(current_directory));
    ls_al(path);

}

void ls_l(char* path) {
    DIR* dir;
    struct dirent* entry;

    dir = opendir(path);
    if (dir == nullptr) {
        perror("Error opening directory");
        return;
    }

    while ((entry = readdir(dir)) != nullptr) {
        // Skip hidden files
        if (entry->d_name[0] == '.') {
            continue;
        }
        print_file_info(path, entry->d_name);
    }

}

void call_ls_l() {

    char current_directory[128];
    char* path = getcwd(current_directory, sizeof(current_directory));
    ls_l(path);

}

void call_ls_parent(){
    
    const char* parent = "..";
    ls(parent,false);
}

