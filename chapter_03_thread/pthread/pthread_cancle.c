#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

void * callback(void * arg);

int main() 
{
    pthread_t tid;
    int ret = pthread_create(&tid, NULL, callback, NULL);
    if (ret != 0) {
        char * errstr = strerror(ret);
        printf("error1 : %s\n", errstr);
    }
    pthread_cancel(tid);
    for (int i = 0; i < 5; i++) {
        printf("%d\n", i);
    }
    printf("tid : %ld, main thread id : %ld\n", tid, pthread_self());
    pthread_exit(NULL);
    return 0;
}

void * callback(void * arg) 
{
    printf("chid thread id : %ld\n", pthread_self());
    for (int i = 0; i < 100; i++) {
        printf("child : %d\n", i);
    }
    return NULL;
}
