#include <stdio.h>
#include <unistd.h>

int main() 
{
    int seconds = alarm(5);
    printf("seconds = %d\n", seconds);  // 0
    sleep(2);
    seconds = alarm(2);    // 不阻塞
    printf("seconds = %d\n", seconds);  // 3
    while(1) {}
    return 0;
}
