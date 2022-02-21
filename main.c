#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *fp;

int main(int argc, char* argv[]) {
    char* buffer = NULL;
    size_t n = 32;
    int line_count;
    if(argc == 1){
        //interactive
        while(1) { //This needs to be changed, I need a way to signal the end of the input chain
            printf("wish> "); //terminal message
            getline(&buffer, &n, stdin);
            //We have the line content here
	    printf("line%d: %s", ++line_count, buffer);
	    if(strcmp(buffer, "exit\n") == 0){
		    break;
	    }
        }
    } else if(argc == 2) {
        //batch
        fp = fopen(argv[1], "r");
        while(!feof(fp)){
            getline(&buffer, &n, fp);
            line_count++;
            printf("line%d: %s", line_count, buffer);
        }
        printf("\n"); //last line won't have newline attached
    } else {
        printf("Please only include 0 or 1 argument(s) alongside the program call: the name of the file to echo\n");
        return 0;
    }
}
