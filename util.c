#include "util.h"

void del_newline(char *str) {
    int length = strlen(str);
    str[length-1] = '\0';
}

char** split_array(char* str) {
    if(str == NULL) {
        return NULL;
    }
    char **ret = malloc(100);
    char *temp_string = "";
    int i = 0;
    int temp_counter = 0;
    while(1){
        temp_string = strsep(&str," ");
        if(temp_string == NULL) 
            break;
        if(strcmp(temp_string, "") == 0) //Ignore whitespace "args"
            continue;
        ret[i] = temp_string;
        i++;
    }
    ret[i] = '\0';
    return ret;
}

char* arr_to_str(char* command, char** args) {
    char *ret = malloc(100);
    strcpy(ret, command);
    int i = 0;
    while(args[i] != 0){
        strcat(ret, " ");
        strcat(ret, args[i]);
        i++;
    }
    strcat(ret, "\n\0");
    return ret;
}

/*char** expand_by_delim(char **arr, char* delim) {
    char **ret[20];
    
    int i = 0;
    //Copy array into new array
    for(i = 0; i < MAX_ARGS; i++) {
        if(arr[i] == NULL)
            break;
        strcpy(ret[i], arr[i]);
        char *temp_string = strsep(&ret[i], delim);
        strcpy(ret[i])
    }

    for(i; i < MAX_ARGS; i++) {

    }
}*/

// For testing purposes
void print_char_array(char** arr) {
    for(int i = 0; i < MAX_ARGS; i++) {
        if(arr[i] != NULL) {
            printf(">>> array[%d] = %s\n", i, arr[i]);
        } else break;
    }
}