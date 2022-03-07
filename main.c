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
    // First try to separate by parallel
    int pid_para = -1;
    char *parallel_command;
    char* shell_call = strsep(&str, "&");
    //printf("shell call %sstr %s\n", shell_call, str);
    if(str != NULL){
        pid_para = fork();
        if(pid_para == 0) {
            parallel_command = malloc(100);
            strcpy(parallel_command, str);
            run_command(parallel_command);
            exit(0);
        } else {
            strcat(shell_call, "\n");
        }
    }
    str = shell_call;

    char *command = "";
    char **args; 
    del_newline(str);



    while(strcmp(command, "") == 0) {
        // Find command while ignoring whitespace
        command = strsep(&str," ");
        if(command == NULL)
            break;
    }

    /*char* cmd_parallel = strsep(&str, "&");
    if(cmd_parallel != NULL) 
        command = cmd_parallel;
*/
    args = split_array(str);

    if(command == NULL && args == NULL) {
        // Pulled an empty line
        return;
    }

    //printf("cmd %s\n", command);
    //print_char_array(args);

/*
    int pid_para = -1;
    int para_index = check_for_parallel(args);
    printf(">para index %d\n", para_index);
    */
    if(strcmp(command, "&") == 0) 
        return;
    /*
    if(para_index != -1) {

        pid_para = fork();
        if(pid_para == 0) {
            // Run command after the &   

            if(args[para_index + 1] == NULL) {
                // & without parallel command to run
                //printf("> & WITHOUT PARALLEL COMMAND\n");
                exit(0);
            }

            // Variable Construction
            char* para_arr[para_index];
            for(int i = para_index; i < MAX_ARGS; i++) {
                if(args[para_index+i+2] == NULL) { //i think this args call might be wrong 
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
            /*char* exec_arr[MAX_ARGS];
            char* exec_str = malloc(100);
            build_exec_vars(exec_arr, exec_str, para_arr, command);
            
            //print_char_array(exec_arr);
            //execute(exec_str, exec_arr, -1);
            char *str = arr_to_str(command, para_arr);
            printf("str = %s", str);
            run_command(str);
            exit(0);
        } else {
            // Continue with first command, overwrite rest of args to NULL
            //waitpid(pid_para, 0, 0); //remove this shittt
            //printf("PARENT TRAP\n");
            for(int i = para_index; i < MAX_ARGS; i++) {
                if(args[i] != NULL)
                    args[i] = NULL;
            }
        }
    }
    */

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
    if(pid_para != -1){
        //printf(">> cmd = %s, pid_para = %d\n", command, pid_para);
        waitpid(pid_para, 0, 0);
    }
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
