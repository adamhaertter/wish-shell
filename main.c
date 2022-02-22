#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

FILE *fp;
char* default_path[] = {"/bin", "/user/bin"};
char **path = default_path;
char **args;

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
        while(1) { //This needs to be changed, I need a way to signal the end of the input chain
            printf("wish> "); //terminal message
            getline(&buffer, &n, stdin);
            
            read_command(buffer);
            
            //printf("line%d: %s", ++line_count, buffer); //echo function
	    if(strcmp(buffer, "exit\n") == 0){
		    break;
	    }
        }
    } else if(argc == 2) {
        //batch
        fp = fopen(argv[1], "r");
        while(1){
            getline(&buffer, &n, fp);
            //printf("line%d: %s", ++line_count, buffer);
            
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
    char *command, *temp_string; //TODO delete temp? do we still use it?
    //char **args; //This doesnt fix our issue
    del_newline(str);

    command = strsep(&str," ");
    args = split_array(str);
    /*
    printf("command: %s | ", command);
    if(args !=NULL)
        printf("%s", args[0]);
    else
        printf("[null]");
    printf(" | %s", *path);
    printf("\n");
    */
    //functionality
    if(strcmp(command, "exit")==0) {
        if(args != NULL)    {
            //If exit has an argument
            print_error();
        } 
        //good exit
        exit(0);   
    } else 
    if(strcmp(command, "path")==0){
        //split arguments into path
        //printf("path detected, assigning\n");
        //path = args;
        
        /* THIS WORKS!!!!!!!!!!!
        path[0] = malloc(100);
        strcpy(path[0], args[0]);
        printf("path assigned\n");
        */
        
        //args = NULL;
        // The memory allocated in split_array is being freed at the end of this run,
        // so we can't store it in
        //path = realloc(path, 100);
        //free(path);
        if(args == NULL) {
            //printf("I see args is null, attempting to set path[0]\n");
            path[0] = NULL;
            //printf("path[0] now NULL\n");
        } else {
                //printf("path loop ");
            for(int i = 0; i < 10; i++) {            
                //printf("%d ", i);
                if(args[i] == NULL) {
                    path[i] = NULL;
                    break;
                }
                path[i] = malloc(30*sizeof(char));
                strcpy(path[0], args[0]);
                if(args[i] == NULL)
                    break;
            }
            //printf("\n");
        }

        //printf("new path: %s \n", path[0]);
        return;
    } else 
    if(strcmp(command, "cd")==0){
        //custom cd call
        if(args == NULL || args[1] != NULL)
            print_error();
        chdir(args[0]);
        return;
    } /*else 
    if(strcmp(command, "ls") == 0) {
        path[0] = "/bin/ls";
    } */
    //TODO access checks

    //printf("pre-command build check-in\n");

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

    char* exec_str = malloc(100);
    if(path[0] != NULL) {
        strcpy(exec_str, path[0]);
        strcat(exec_str, "/");
    }
    else
        strcpy(exec_str, "");
    strcat(exec_str, command);
    //printf("EXEC_STR: %s\n", exec_str);
    
    //printf("access val %d\n", access(command, X_OK));

    //printf("pre-fork check-in\n");
    int result = 0;
    int pid = fork();
    if(pid == 0){
        //kiddie pool
        result = execv(exec_str, exec_arr); //TODO FIX THIS, SEARCH ALL DIRS
        //printf("Error caught w/ value %s\n", strerror(errno));
        if(result)
            print_error();
        
    } else {
        //adult swim
        waitpid(pid, 0, 0);
        free(exec_str);
        return;
    }
    //printf("execv results in: %d", result);
    //exit(0);
        //printf("Hi, I am %d and I'm causing problems w/ value %s\n", pid, strerror(errno));
        //if(errno != 0)
        //    print_error();
    //TODO
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
