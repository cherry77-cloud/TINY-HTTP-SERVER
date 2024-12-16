#include <signal.h>
#include <stdio.h>

int main() 
{
    sigset_t set;
    // Initialize the signal set to empty
    sigemptyset(&set);
    // Check if SIGINT is a member of the set
    int ret = sigismember(&set, SIGINT);
    if (ret == 0) {
        printf("SIGINT is not blocked\n");
    } else if (ret == 1) {
        printf("SIGINT is blocked\n");
    }

    // Add SIGINT and SIGQUIT to the signal set
    sigaddset(&set, SIGINT);
    sigaddset(&set, SIGQUIT);
    
    // Check again if SIGINT is a member of the set
    ret = sigismember(&set, SIGINT);
    if (ret == 0) {
        printf("SIGINT is not blocked\n");
    } else if (ret == 1) {
        printf("SIGINT is blocked\n");
    }

    // Check if SIGQUIT is a member of the set
    ret = sigismember(&set, SIGQUIT);
    if (ret == 0) {
        printf("SIGQUIT is not blocked\n");
    } else if (ret == 1) {
        printf("SIGQUIT is blocked\n");
    }

    // Remove SIGQUIT from the signal set
    sigdelset(&set, SIGQUIT);
    // Check again if SIGQUIT is a member of the set
    ret = sigismember(&set, SIGQUIT);
    if (ret == 0) {
        printf("SIGQUIT is not blocked\n");
    } else if (ret == 1) {
        printf("SIGQUIT is blocked\n");
    }
    return 0;
}
