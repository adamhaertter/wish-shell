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
        while(1){
            getline(&buffer, &n, fp);           
            if(feof(fp))
                break;
            run_command(buffer);
        }
    } else {
        printf("Please only include 0 or 1 argument(s) alongside the program call: the name of the file to echo\n");
        return 0;
    }
}

void run_command(char* str) {
    char *command;
    char **args; 
    del_newline(str);

    command = strsep(&str," ");
    args = split_array(str);
    /*
    printf(">command: %s\n", command);
    if(args == NULL)
        printf(">args is null\n");
    else
        printf(">args[0]: %s\n", args[0]);
    if(path == NULL)
        printf(">path is null\n");
    else
        printf(">path[0]: %s\n", *path);
*/
    // Function Calls
    if(strcmp(command, "exit")==0) {
        wish_exit(args);
    } else 
    if(strcmp(command, "path")==0){
        wish_path(args, path);

        //printf(">>path addr: %p\n", path);
        if(path == NULL)
        //printf("path is null in comm.c\n");

        return;
    } else 
    if(strcmp(command, "cd")==0){
        wish_cd(args);
        return;
    }

    // Variable Construction
    char* exec_arr[MAX_ARGS];
    char* exec_str = malloc(100);
    build_exec_vars(exec_arr, exec_str, args, command);
    //printf(">exec vars built\n");
    // Execution & Fork
    int result = 0;
    int pid = fork();
    if(pid == 0){
        // Child process
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

    //printf(">Commands built\n");
    // Search directories for access
    for(i = 0; i < MAX_ARGS; i++) {
        if(path[i] != NULL) {
            //printf(">if passed\n");
            strcpy(exec_str, path[i]);
            strcat(exec_str, "/");
        } else
            strcpy(exec_str, "");
        strcat(exec_str, exec_arr[0]);

        if(!access(exec_str, X_OK))
            break;
    }
}
