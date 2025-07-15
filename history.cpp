#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <vector>
#include <fstream>
#include <deque>

using namespace std;

deque<string> history_comm;  // use deque for efficient pop from front

// Called once at startup
void load_history_from_file(const string& filename) {
    ifstream infile(filename);
    if (!infile.is_open()) {
        // Create empty file if doesn't exist
        ofstream outfile(filename);
        outfile.close();
        return;
    }

    string line;
    while (getline(infile, line)) {
        if (!line.empty()) {
            history_comm.push_back(line);
            if (history_comm.size() > 20) {
                history_comm.pop_front();
            }
        }
    }
    infile.close();
}

// Called each time a command is entered
void add_to_history(const string& command, const string& filename) {
    if (command.empty()) return;

    history_comm.push_back(command);
    if (history_comm.size() > 20) {
        history_comm.pop_front();
    }

    // Rewrite file with updated 20 commands
    ofstream outfile(filename, ios::trunc);
    for (const string& cmd : history_comm) {
        outfile << cmd << '\n';
    }
    outfile.close();
}

void call_history() {
    int start = max(0, (int)history_comm.size() - 10);
    for (int i = start; i < history_comm.size(); ++i) {
        cout << history_comm[i] << '\n';
    }
}

void call_history1(int num) {
    if (num < 0) {
        cout << "history: number must be non-negative\n";
        return;
    }

    int cnt = min(num, (int)history_comm.size());
    int start = history_comm.size() - cnt;

    for (int i = start; i < history_comm.size(); ++i) {
        cout << history_comm[i] << '\n';
    }
}
