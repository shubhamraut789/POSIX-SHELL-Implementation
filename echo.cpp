#include <iostream>
#include <string>
#include <cctype>  // For isspace()

using namespace std;

void call_echo(string& str) {
    string s = "";
    bool flag = false;  // This will track if we are inside quotes
    bool started = false;  // This will track if we've started adding non-space characters
    long i = 0;

    while (i < str.size() && isspace(str[i])) {
        i++;
    }

    // Skip the "echo" command itself
    string command = "echo";
    if (str.substr(i, command.size()) == command) {
        i += command.size();
    }else i=i;

    while (i < str.size() && isspace(str[i])) {
        i++;
    }

    // Iterate over the remaining input string
    for (; i < str.size(); i++) {
        if (str[i] == '"') {
            flag = !flag;  // Toggle the flag when encountering a quote
            started = true;
            continue;
        }

        // Handle spaces and tabs outside quotes
        if(!flag && isspace(str[i])){
            if(started && !s.empty() && s.back() != ' '){
                s += ' ';
            }
        }else{
            s += str[i];
            started = true;
        }
    }

    // Remove trailing space, if any
    if (!s.empty() && s.back() == ' ') {
        s.pop_back();
    }

    cout << s << '\n';
}
