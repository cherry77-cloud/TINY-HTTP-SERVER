#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

int main() 
{    
    int shmid = shmget(100, 4096, IPC_CREAT|0664);
    printf("shmid : %d\n", shmid);
    void * ptr = shmat(shmid, NULL, 0);
    char * str = "helloworld";
    memcpy(ptr, str, strlen(str) + 1);
    getchar();
    shmdt(ptr);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}
