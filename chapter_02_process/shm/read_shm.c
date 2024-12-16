#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

int main() 
{    
    int shmid = shmget(100, 0, IPC_CREAT);
    printf("shmid : %d\n", shmid);
    void * ptr = shmat(shmid, NULL, 0);
    printf("%s\n", (char *)ptr);
    getchar();
    shmdt(ptr);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}
