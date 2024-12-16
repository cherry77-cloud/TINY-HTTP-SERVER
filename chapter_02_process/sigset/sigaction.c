#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void myalarm(int num);

int main() 
{
    struct sigaction act;
    act.sa_flags = 0;
    act.sa_handler = myalarm;
    sigemptyset(&act.sa_mask);
    if (sigaction(SIGALRM, &act, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }
    struct itimerval new_value;
    new_value.it_interval.tv_sec = 2;
    new_value.it_interval.tv_usec = 0;
    new_value.it_value.tv_sec = 3;
    new_value.it_value.tv_usec = 0;
  
    int ret = setitimer(ITIMER_REAL, &new_value, NULL);
    printf("Timer started...\n");
    if (ret == -1) {
        perror("setitimer");
        exit(EXIT_FAILURE);
    }
    while(1);
    return 0;
}

void myalarm(int num) 
{
    printf("Received signal number: %d\n", num);
}
