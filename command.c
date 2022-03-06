#include "command.h"

void wish_exit(char** args) {
    if(args != NULL)    {
        // If exit has an argument
        print_error();
    } 
    // good exit
    exit(0);   
}

void wish_path(char **args, char **path) {
    // split arguments into path
    if(args == NULL) {
        path[0] = NULL;
    } else {
        for(int i = 0; i < MAX_ARGS; i++) {    
            if(args[i] == NULL) {
                break;
            }
            path[i] = malloc(30*sizeof(char));
            strcpy(path[i], args[i]);
        }
    }
}

void wish_cd(char **args) {
    // custom cd call
    if(args == NULL || args[1] != NULL)
        print_error();
    chdir(args[0]);
}

int check_for_redirect(char** args) {
    if(args == NULL)
        return -1;
    //Check for separate argument
    for(int i = 0; i < MAX_ARGS; i++) {
        if(args[i] == NULL)
            break;
        else if(strcmp(args[i], ">") == 0)
            return i;
    }

    //Check for contained within argument
    for(int i = MAX_ARGS; i >= 0; i--) {
        if(args[i] == NULL)
            continue;
        for(int j = 0; j < strlen(args[i]); j++){
            if(args[i][j] == '>') {
                char *destination = malloc(100);
                strcpy(destination, args[i]);
                args[i] = strsep(&destination, ">");
                args[i+1] = ">";
                args[i+2] = destination;
                return i+1;
            }
        }
    }

    return -1;
}

void wish_redirect(char **args, int redir_index) {
    // args[redir_index] is guaranteed to be >
    if(args[redir_index+1] == NULL || args[redir_index+2] != NULL) {
        // No path to redirect or multiple redirects
        print_error();
    } else {
        freopen(args[redir_index+1], "w", stdout);
        // Set the redirect args to NULL so they don't get passed to the command
        args[redir_index] = NULL;
        args[redir_index+1] = NULL;
    }

}

int check_for_parallel(char** args) {
    if(args == NULL)
        return -1;
    //Check for separate argument
    for(int i = 0; i < MAX_ARGS; i++) {
        if(args[i] == NULL)
            break;
        else if(strcmp(args[i], "&") == 0)
            return i;
    }

    //Check for contained within argument
    for(int i = 0; i < MAX_ARGS; i++) {
        if(args[i] == NULL)
            break;
        printf(">>args[%d] = %s\n", i, args[i]);
        for(int j = 0; j < strlen(args[i]); j++){
            if(args[i][j] == '&') {
                char *destination = malloc(100);
                strcpy(destination, args[i]);
                args[i] = strsep(&destination, "&");
                args[i+1] = "&";
                args[i+2] = destination;
                return i+1;
            }
        }
    }

    return -1;
}
