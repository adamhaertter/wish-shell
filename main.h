#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_ARGS 10

FILE *fp;
char* default_path[MAX_ARGS];
char **path = default_path;

void run_command(char* str);
void execute(char* exec_str, char** exec_arr, int redir_index);
void print_error();
void print_error_fatal();
void build_exec_vars(char** exec_arr, char* exec_str, char** args, char* command);