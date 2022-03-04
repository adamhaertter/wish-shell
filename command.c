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
        for(int i = 0; i < 10; i++) {    
            if(args[i] == NULL) {
                //strcpy(path[i], '\0');
                break;
            }
            path[i] = malloc(30*sizeof(char));
            strcpy(path[i], args[i]);
            //printf(">>path[%d] = %s\n", i, path[i]);
            //printf(">>path addr: %p\n", path);
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
    for(int i = 0; i < MAX_ARGS; i++) {
        if(args[i] == NULL)
            break;
        else if(strcmp(args[i], ">") == 0)
            return i;
    }
    return -1;
}

void wish_redirect(char **args, int redir_index) {
    // args[0] is guaranteed to be >
    if(args[redir_index+1] == NULL) {
        // No path to redirect
        print_error();
    } else {
        freopen(args[redir_index+1], "w", stdout);
    }

}