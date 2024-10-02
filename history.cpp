#include <iostream>
#include <unistd.h>
#include <syscall.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <algorithm>
#include <map>
#include <errno.h>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

vector<string> history_comm;

void add_to_history(const string& command,const string& filename) {
    
    if(history_comm.size() >= 20) {
        history_comm.erase(history_comm.begin());
    }

    //history_comm.push_back(command);

    int fd = open(filename.c_str(), O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    if(fd == -1){
        perror("Error while opening history.txt for writing");
        return;
    }

    string data = command + '\n';
    if(write(fd, data.c_str(), data.size()) == -1) {
        perror("Error while writing to history.txt");
    }

    ifstream file(filename);
    
    if(!file.is_open()) {
        perror("Error while opening file history.txt");
    }
    
    string line;

    while(getline(file, line)) {
        history_comm.push_back(line);
    }

}

void call_history(){

        if(history_comm.size() >= 20){
            history_comm.erase(history_comm.begin());
        }
        int start;

        if(history_comm.size() > 10){
            start = history_comm.size() - 10;
        }else start = 0;

        for(int i = start; i < history_comm.size(); i++){
            cout << history_comm[i] << '\n';
        }

}

void call_history1(int num){

        if(num < 0){
            cout<<"history num is negative"<<'\n';
            return ;
        }

        while(history_comm.size() > 20){
            history_comm.erase(history_comm.begin());
        }
        int start;
        int cnt;
        if(num == -1) cnt = 10;
        else cnt = num;

        if(cnt > history_comm.size()) cnt = history_comm.size();

        start = history_comm.size() - cnt;

        for(int i = start; i < history_comm.size(); i++){
            cout << history_comm[i] << '\n';
        }
        history_comm.clear();

}