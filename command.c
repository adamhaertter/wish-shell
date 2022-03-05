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
            //printf("%c ", args[i][j]);
            if(args[i][j] == '>') {
                char *destination = malloc(100); //MIGHT CAUSE MEM LEAK
                strcpy(destination, args[i]);
                args[i] = strsep(&destination, ">");
                args[i+1] = ">";
                args[i+2] = destination;
                return i+1;
                /* 
                //I DONT KNOW IF THIS IS RIGHT, but you have to split args[i] into 3 args if they're one string (unfinished)
                char **split_arg = args[i];
                arg[i] = strsep(split_arg,">");
                args[i] = temp_string;
                i++;
                */
            }
        }
        //printf("\n");
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