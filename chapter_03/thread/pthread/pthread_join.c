#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

int value = 10;
void * callback(void * arg);

int main() 
{
    pthread_t tid;
    int ret = pthread_create(&tid, NULL, callback, NULL);
    if (ret != 0) {
        char * errstr = strerror(ret);
        printf("Error : %s\n", errstr);
    }
    for (int i = 0; i < 5; i++) {
        printf("%d\n", i);
    }
    printf("Thread ID : %ld, Main thread ID : %ld\n", tid, pthread_self());
    int * thread_retval;
    ret = pthread_join(tid, (void **)&thread_retval);
    if (ret != 0) {
        char * errstr = strerror(ret);
        printf("Error : %s\n", errstr);
    }
    printf("Exit data : %d\n", *thread_retval);
    printf("Successfully reclaimed child thread resources!\n");
    pthread_exit(NULL);
    return 0; 
}

void * callback(void * arg) 
{
    printf("Child thread ID : %ld\n", pthread_self());
    pthread_exit((void *)&value);
}
