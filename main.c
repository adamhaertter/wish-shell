#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

FILE *fp;
char* default_path[] = {"/bin", "/user/bin"};
char **path = default_path;

void read_command(char* str);
void print_error();
void del_newline(char* str);
char** split_array(char* str);

int main(int argc, char* argv[]) {
    char* buffer = NULL;
    size_t n = 32;
    int line_count;
    if(argc == 1){
        //interactive
        while(1) { 
            printf("wish> "); //terminal message
            getline(&buffer, &n, stdin);
            
            read_command(buffer);
	    if(strcmp(buffer, "exit\n") == 0){
		    break;
	    }
        }
    } else if(argc == 2) {
        //batch
        fp = fopen(argv[1], "r");
        while(1){
            getline(&buffer, &n, fp);
            
            if(feof(fp))
                break;
            
            read_command(buffer);
        }
    } else {
        printf("Please only include 0 or 1 argument(s) alongside the program call: the name of the file to echo\n");
        return 0;
    }
}

void read_command(char* str) {
    char *command;
    char **args; 
    del_newline(str);

    command = strsep(&str," ");
    args = split_array(str);

    //functionality
    if(strcmp(command, "exit")==0) {
        if(args != NULL)    {
            // If exit has an argument
            print_error();
        } 
        // good exit
        exit(0);   
    } else 
    if(strcmp(command, "path")==0){
        // split arguments into path
        if(args == NULL) {
            path[0] = NULL;
        } else {
            for(int i = 0; i < 10; i++) {    
                if(args[i] == NULL) {
                    path[i] = NULL;
                    break;
                }
                path[i] = malloc(30*sizeof(char));
                strcpy(path[0], args[0]);
                if(args[i] == NULL)
                    break;
            }
        }
        return;
    } else 
    if(strcmp(command, "cd")==0){
        // custom cd call
        if(args == NULL || args[1] != NULL)
            print_error();
        chdir(args[0]);
        return;
    }

    // Building full list of command + args to run
    char* exec_arr[10];
    
    exec_arr[0] = command;
    int i = 0;
    if(args != NULL) {
        for(i = 0; i < 10; i++) {
            exec_arr[i+1] = args[i];
            if(args[i] == NULL)
                break;
        }
    }
    exec_arr[i+1] = NULL;

    // Search directories for access
    char* exec_str = malloc(100);
    for(i = 0; i < 10; i++) {
        if(path[i] != NULL) {
            strcpy(exec_str, path[i]);
            strcat(exec_str, "/");
        } else
            strcpy(exec_str, "");
        strcat(exec_str, command);

        if(!access(exec_str, X_OK))
            break;
    }

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

void del_newline(char *str) {
    int length = strlen(str);
    str[length-1] = '\0';
}

char** split_array(char* str) {
    if(str == NULL) {
        return NULL;
    }
    char **ret = malloc(100);
    char *temp_string;
    int i = 0;
    while(temp_string != NULL){
        temp_string = strsep(&str," ");
        ret[i] = temp_string;
        i++;
    }
    ret[i] = '\0';
    return ret;
}
