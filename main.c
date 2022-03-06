#include "main.h"
#include "util.c"
#include "command.c"

int main(int argc, char* argv[]) {
    char* buffer = NULL;
    size_t n = 32;
    int line_count;
    default_path[0] = "/bin";
    default_path[1] = "";
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

    //printf("cmd %s\n", command);
    //print_char_array(args);

    int pid_para = -1;
    int para_index = check_for_parallel(args);
    
    if(strcmp(command, "&") == 0) 
        return;
    
    if(para_index != -1) {
 
        
    if(args[para_index + 1] == NULL) {
        // & without parallel command to run
        return;
    }

        pid_para = fork();
        if(pid_para == 0) {
            // Run command after the &   
            // Variable Construction
            char* para_arr[para_index];
            for(int i = para_index; i < MAX_ARGS; i++) {
                if(args[para_index+i+2] == NULL) {
                    para_arr[i] = NULL;
                    break;
                }
                para_arr[i] = malloc(30);
                strcpy(para_arr[i], args[para_index+2+i]);
                //printf("> para_arr[%d] = %s\n", i, para_arr[i]);
            } 
            //printf("out of loop\n");
            command = args[pid_para + 1];
            //printf("cmd %s\n", command);
            char* exec_arr[MAX_ARGS];
            char* exec_str = malloc(100);
            build_exec_vars(exec_arr, exec_str, para_arr, command);
            print_char_array(exec_arr);
            //execute(exec_str, exec_arr, -1);
            return;
        } else {
            // Continue with first command, overwrite rest of args to NULL
            waitpid(pid_para, 0, 0); //remove this shittt
            //printf("PARENT TRAP\n");
            for(int i = para_index; i < MAX_ARGS; i++) {
                if(args[i] != NULL)
                    args[i] = NULL;
            }
        }
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
        execute(exec_str, exec_arr, redir_index);
    } else {
        // Parent process
        waitpid(pid, 0, 0);
        free(exec_str);
    }
    if(pid_para != -1)
        waitpid(pid_para, 0, 0);
    return;
}

void execute(char* exec_str, char** exec_arr, int redir_index) {
        // Redirect if neccessary
        if(redir_index != -1)
            wish_redirect(exec_arr, redir_index+1);

        int result = execv(exec_str, exec_arr);

        if(result)
            print_error();
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
