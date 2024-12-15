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
