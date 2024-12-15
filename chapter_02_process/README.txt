1. fork
🎡 系统允许一个进程创建新进程, 新进程即为子进程, 子进程还可以创建新的子进程, 形成进程树结构模型.
🎡 使用GDB调试的时候, GDB默认只能跟踪一个进程, 可以在 fork 函数调用之前, 通过指令设置GDB调试工具跟踪父进程或者是跟踪子进程, 默认跟踪父进程.
🎡 设置调试父进程或者子进程 set follow-fork-mode [parent(默认) | child]
🎡 设置调试模式: set detach-on-fork [on | off] 默认为on, 表示调试当前进程的时候, 其它的进程继续运行, 如果为off, 调试当前进程的时候, 
其他进程被 GDB 挂起
🎡 查看调试的进程: info inferiors
🎡 切换当前调试的进程: inferior id
🎡 使进程脱离GDB调试: detach inferiors id
🎡 fork() 创建子进程时，通过写时拷贝机制共享父子进程的内存空间，只有在写操作发生时才复制内存页面，提高了效率和性能。
🎡 fork() 后，父子进程共享相同的文件描述符，这些描述符指向相同的文件表项，文件偏移量和文件状态信息也是共享的。


2. exec函数族
🍞 exec 函数族包括多个函数，它们的主要区别在于参数的传递方式。所有这些函数都会用新程序替换当前进程的映像，但不会创建新进程。常见的 exec 
函数包括：execl() execv() execle() execve() execlp() execvp() execvpe()
🍞 int execl(const char *path, const char *arg, ..., (char *) NULL);
🍞 int execv(const char *path, char *const argv[]);
🍞 int execle(const char *path, const char *arg, ..., (char *) NULL, char *const envp[]);
🍞 int execve(const char *path, char *const argv[], char *const envp[]);
🍞 int execlp(const char *file, const char *arg, ..., (char *) NULL);
🍞 int execvp(const char *file, char *const argv[]);
🍞 int execvpe(const char *file, char *const argv[], char *const envp[]);


3. 检查已回收子进程的退出状态
🦔 WIFEXITED(status): 如果子进程通过调用 exit 或者一个 return 正常终止, 就返回真.
🦔 WEXITSTATUS(status): 返回一个正常终止的子进程的退出状态. 只有在 WIFEXITED()返回一个正常终止的子进程的退出状态为真时, 才会定义这个状态.
🦔 WIFSIGNALED(status): 如果子进程是因为一个未被捕获的信号终止的, 那么就返回真.
🦔 WTERMSIG(status): 返回导致子进程终止的信号的编号. 只有在WIFSIGNALED()返回为真时, 才定义这个状态.
🦔 WIFSTOPPED(status): 如果引起返回的子进程当前是停止的, 那么就返回真.
🦔 WSTOPSIG(status): 返回引起子进程终止的信号的编号. 只有在 WIFSTOPPED()返回为真时, 才定义这个状态.
🦔 WIFCONTINUED(status): 如果子进程收到 SIGCOUT 信号重新启动, 则返回真.


4. 管道
◼ 管道其实是一个在内核内存中维护的缓冲器，这个缓冲器的存储能力是有限的，不同的操作系统大小不一定相同。
◼ 管道拥有文件的特质：读操作、写操作，匿名管道没有文件实体，有名管道有文件实体，但不存储数据。可以按照操作文件的方式对管道进行操作。
◼ 一个管道是一个字节流，使用管道时不存在消息或者消息边界的概念，从管道读取数据的进程可以读取任意大小的数据块，而不管写入进程写入管道
的数据块的大小是多少。
◼ 通过管道传递的数据是顺序的，从管道中读取出来的字节的顺序和它们被写入管道的顺序是完全一样的。在管道中的数据的传递方向是单向的，一端
用于写入，一端用于读取，管道是半双工的。
◼ 从管道读数据是一次性操作，数据一旦被读走，它就从管道中被抛弃，释放空间以便写更多的数据，在管道中无法使用 lseek() 来随机的访问数据。
◼ 匿名管道只能在具有公共祖先的进程（父进程与子进程，或者两个兄弟进程，具有亲缘关系）之间使用。


5. 匿名管道
◼ 创建匿名管道  int pipe(int pipefd[2]);
◼ 查看管道缓冲大小命令  ulimit –a
◼ 查看管道缓冲大小函数  long fpathconf(int fd, int name);


6. 有名管道
◼ 通过命令创建有名管道  mkfifo 名字
◼ 通过函数创建有名管道  int mkfifo(const char *pathname, mode_t mode);
◼ 一旦使用 mkfifo 创建了一个 FIFO, 就可以使用 open 打开它, 常见的文件 I/O 函数都可用于 fifo. 如: close, read, write, unlink 等。
◼ FIFO 严格遵循先进先出（First in First out），对管道及 FIFO 的读总是从开始处返回数据，对它们的写则把数据添加到末尾。
它们不支持诸如 lseek() 等文件定位操作。
