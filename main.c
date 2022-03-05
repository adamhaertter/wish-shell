#include "main.h"
#include "util.c"
#include "command.c"

int main(int argc, char* argv[]) {
    char* buffer = NULL;
    size_t n = 32;
    int line_count;
    if(argc == 1){
        //interactive
        while(1) { 
            printf("wish> "); //terminal message
            getline(&buffer, &n, stdin);          
            run_command(buffer);
        }
    } else if(argc == 2) {
        //batch
        fp = fopen(argv[1], "r");
        if(fp == NULL)
            print_error_fatal();
        while(1){
            int getline_result = getline(&buffer, &n, fp);   
            if(feof(fp)) {
                break;
            }
            fseek(fp, 0L, SEEK_CUR);
            run_command(buffer);
        }
        fclose(fp);
    } else {
        print_error_fatal();
    }
}

void run_command(char* str) {
    char *command = "";
    char **args; 
    del_newline(str);

    while(strcmp(command, "") == 0) {
        // Find command while ignoring whitespace
        command = strsep(&str," ");
        if(command == NULL)
            break;
    }
    
    args = split_array(str);

    if(command == NULL && args == NULL) {
        // Pulled an empty line
        return;
    }

    // Function Calls
    if(strcmp(command, "exit")==0) {
        wish_exit(args);
    } else 
    if(strcmp(command, "path")==0){
        wish_path(args, path); return;
    } else 
    if(strcmp(command, "cd")==0){
        wish_cd(args);
        return;
    }
    
    int redir_index = check_for_redirect(args);    

    // Variable Construction
    char* exec_arr[MAX_ARGS];
    char* exec_str = malloc(100);
    build_exec_vars(exec_arr, exec_str, args, command);
    
    // Execution & Fork
    int result = 0;
    int pid = fork();
    if(pid == 0){
        // Child process
        
        // Redirect if neccessary
        if(redir_index != -1)
            wish_redirect(exec_arr, redir_index+1);

        result = execv(exec_str, exec_arr);

        if(result)
            print_error();
        
    } else {
        // Parent process
        waitpid(pid, 0, 0);
        free(exec_str);
        return;
    }
}

void print_error() {
    char *err = "An error has occurred\n";
    write(STDERR_FILENO, err, strlen(err));
    exit(0);
}

void print_error_fatal() {
    char *err = "An error has occurred\n";
    write(STDERR_FILENO, err, strlen(err));
    exit(1);
}

void build_exec_vars(char** exec_arr, char* exec_str, char** args, char* command) {
    // Building full list of command + args to run
    exec_arr[0] = command;
    int i = 0;
    if(args != NULL) {
        for(i = 0; i < MAX_ARGS; i++) {
            exec_arr[i+1] = args[i];
            if(args[i] == NULL)
                break;
        }
    }
    exec_arr[i+1] = NULL;

    // Search directories for access
    for(i = 0; i < MAX_ARGS; i++) {
        if(path[i] != NULL) {
            strcpy(exec_str, path[i]);
            strcat(exec_str, "/");
        } else
            strcpy(exec_str, "");
        strcat(exec_str, exec_arr[0]);

        if(!access(exec_str, X_OK))
            break;
    }
}
