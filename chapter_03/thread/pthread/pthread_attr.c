#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

void * callback(void * arg);

int main() 
{
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_t tid;
    int ret = pthread_create(&tid, &attr, callback, NULL);
    if (ret != 0) {
        char * errstr = strerror(ret);
        printf("error1 : %s\n", errstr);
    }
    size_t size;
    pthread_attr_getstacksize(&attr, &size);
    printf("thread stack size : %ld\n", size);
    printf("tid : %ld, main thread id : %ld\n", tid, pthread_self());
    pthread_attr_destroy(&attr);
    pthread_exit(NULL);
    return 0;
}

void * callback(void * arg) 
{
    printf("chid thread id : %ld\n", pthread_self());
    return NULL;
}
