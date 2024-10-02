#include <vector>
#include <string>
using namespace std;

void displayPrompt();

vector<string> tokenize_ip(const string& ip);

void call_ls_a();
void call_ls();
void call_ls_al();
void call_ls_l();
void call_ls_parent();

void call_cd(const vector<string>& tokens);

void call_echo(string& str);

void call_pwd();

void call_search(string& searchString);

void add_to_history(const string& command, const string& filename);

void call_history();
void call_history1(int num);

void handle_redirection(vector<string>& tokens);

void call_pipes(const vector<string>& commands);

void call_pinfo(int num_of_args, vector<string> args);
