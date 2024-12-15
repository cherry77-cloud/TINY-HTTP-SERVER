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
            printf("parent, pid = %d\n", getpid());
            sleep(1);
            int st;
            int ret = waitpid(-1, &st, WNOHANG);
            if (ret == -1) {
                break;  // Exit loop if no child is left
            } else if (ret == 0) {
                continue;  // No child has exited, continue waiting
            } else if (ret > 0) {
                if (WIFEXITED(st)) {
                    printf("Exited with status: %d\n", WEXITSTATUS(st));
                }
                if (WIFSIGNALED(st)) {
                    printf("Killed by signal: %d\n", WTERMSIG(st));
                }
                printf("child died, pid = %d\n", ret);
            }
        }
    } else if (pid == 0) {
        while(1) {
            printf("child, pid = %d\n",getpid());    
            sleep(1);       
        }
        exit(0);
    }
    return 0; 
}
