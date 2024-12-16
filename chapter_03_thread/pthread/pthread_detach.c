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
    printf("tid : %ld, main thread id : %ld\n", tid, pthread_self());
    ret = pthread_detach(tid);
    if (ret != 0) {
        char * errstr = strerror(ret);
        printf("error2 : %s\n", errstr);
    }
    pthread_exit(NULL);
    return 0;
}

void * callback(void * arg) 
{
    printf("chid thread id : %ld\n", pthread_self());
    return NULL;
}
