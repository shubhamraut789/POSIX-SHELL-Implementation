This project implements a user-defined shell program supporting built-in commands, system commands, background processes, input/output redirection, pipes, and more. Key features include:

Features:
Custom Shell Prompt: Displays <username>@<system_name>:<current_directory> with ~ for the home directory.
Built-in Commands:
cd (with . .. - ~ options).
echo, pwd, ls (supports -a, -l flags).
System Commands: Executes processes in foreground and background (&) with PID display.
pinfo: Displays process details for current or specified PID.
search: Recursively searches files/folders under the current directory.
I/O Redirection: Supports <, >, and >> operators for input and output redirection.
Pipes: Handles multiple pipes (|) for command chaining.
Signal Handling:
CTRL-Z: Stops and backgrounds a foreground process.
CTRL-C: Interrupts a foreground process.
CTRL-D: Exits the shell.
Autocomplete: Implements TAB completion for files and directories.
Command History: Tracks up to 20 commands across sessions, with history and arrow key support.

Restrictions:
No system(), popen(), or curses/ncurses.
Implements custom parsing, redirection, and piping without using advanced libraries like <filesystem>.

Structure
Includes a Makefile for compilation 
