#include <readline/readline.h>
#include <readline/history.h>
#include <iostream>
#include <string>

using namespace std;

// Reads user input with support for:
// - TAB autocomplete (provided by readline)
// - Arrow key history navigation
// - Input editing (←, →, backspace, etc.)
string get_autocomplete_input() {
    // Prompt is handled by your shell separately (displayPrompt()), so we pass empty string
    char* input = readline("");

    // Handle EOF (Ctrl+D)
    if (!input) return "";

    string command(input);

    // Add to shell history (retrievable via UP arrow)
    if (!command.empty()) {
        add_history(input);
    }

    free(input); // readline allocates with malloc
    return command;
}
