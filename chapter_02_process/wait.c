#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() 
{
    pid_t pid;
    for (int i = 0; i < 5; i++) {
        pid = fork();
        if (pid == 0) {
            break;
        }
    }
    if (pid > 0) {
        while(1) {
            printf("Parent process, PID = %d\n", getpid());
            int status;
            int ret = wait(&status);
            if (ret == -1) {
                break;
            }
            if (WIFEXITED(status)) {
                printf("Child exited with status: %d\n", WEXITSTATUS(status));
            }
            if (WIFSIGNALED(status)) {
                printf("Child terminated by signal: %d\n", WTERMSIG(status));
            }
            printf("Child process terminated, PID = %d\n", ret);
            sleep(1);
        }
    }
    else if (pid == 0) {
        while (1) {
            printf("Child process, PID = %d\n", getpid());
            sleep(1);
        }
        exit(0);
    }
    return 0;
}
