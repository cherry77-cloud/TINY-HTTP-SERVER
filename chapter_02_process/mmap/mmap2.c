#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main() 
{
    int fd = open("test.txt", O_RDWR);
    if (fd == -1) {
        perror("open");
        exit(0);
    }
    int len = lseek(fd, 0, SEEK_END);
    int fd1 = open("cpy.txt", O_RDWR | O_CREAT, 0664);
    if (fd1 == -1) {
        perror("open");
        exit(0);
    }
    truncate("cpy.txt", len);
    write(fd1, " ", 1);
    void * ptr = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    void * ptr1 = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd1, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        exit(0);
    }
    if (ptr1 == MAP_FAILED) {
        perror("mmap");
        exit(0);
    }
    memcpy(ptr1, ptr, len);
    munmap(ptr1, len);
    munmap(ptr, len);
    close(fd1);
    close(fd);
    return 0;
}
