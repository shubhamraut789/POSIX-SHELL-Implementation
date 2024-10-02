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
#include "functions.h"

using namespace std;

void call_pwd(){

    char current_directory[128];
    char* status = getcwd(current_directory,sizeof(current_directory));

    if(status){
        cout<<status<<'\n';
    }else cout<<"Can't get current directory path"<< '\n';
    
}