#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() 
{
    int pipefd[2];
    int ret = pipe(pipefd);
    long size = fpathconf(pipefd[0], _PC_PIPE_BUF);
    printf("pipe size : %ld\n", size);
    return 0;
}

// ulimit -a
// real-time non-blocking time  (microseconds, -R) unlimited
// core file size              (blocks, -c) unlimited
// data seg size               (kbytes, -d) unlimited
// scheduling priority                 (-e) 0
// file size                   (blocks, -f) unlimited
// pending signals                     (-i) 878182
// max locked memory           (kbytes, -l) unlimited
// max memory size             (kbytes, -m) unlimited
// open files                          (-n) 1048576
// pipe size                (512 bytes, -p) 8
// POSIX message queues         (bytes, -q) 819200
// real-time priority                  (-r) 0
// stack size                  (kbytes, -s) 8192
// cpu time                   (seconds, -t) unlimited
// max user processes                  (-u) unlimited
// virtual memory              (kbytes, -v) unlimited
// file locks                          (-x) unlimited
