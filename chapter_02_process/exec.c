#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main() 
{
    pid_t pid = fork();
    if (pid < 0) {
        printf("Fork failed!");
        return 1;
    } else if (pid == 0) {
        char *args[] = {(char *)"ls", (char *)"-l", (char *)"/", NULL};
        if (execvp("ls", args) == -1) {
            perror("execvp failed");
            exit(EXIT_FAILURE);
        }
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            printf("Child exited with status, %d\n", WEXITSTATUS(status));
        }
    }
    return 0;
}
