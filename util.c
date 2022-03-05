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
    while(temp_string != NULL){
        temp_string = strsep(&str," ");
        ret[i] = temp_string;
        i++;
        //printf(">>args[%d] = %s\n", i, temp_string);
    }
    ret[i] = '\0';
    return ret;
}

void print_char_array(char** arr) {
    for(int i = 0; i < MAX_ARGS; i++) {
        if(arr[i] != NULL) {
            printf(">>> array[%d] = %s\n", i, arr[i]);
        } else break;
    }
}