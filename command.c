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
                path[i] = NULL;
                break;
            }
            path[i] = malloc(30*sizeof(char));
            strcpy(path[0], args[0]);
            if(args[i] == NULL)
                break;
        }
    }
}

void wish_cd(char **args) {
    // custom cd call
    if(args == NULL || args[1] != NULL)
        print_error();
    chdir(args[0]);
}