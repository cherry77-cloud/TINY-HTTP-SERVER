1. 信号
◼ 信号是 Linux 进程间通信的最古老的方式之一，是事件发生时对进程的通知机制，有时也称之为软件中断，它是在软件层次上对中断机制的一种模拟，
是一种异步通信的方式。信号可以导致一个正在运行的进程被另一个正在运行的异步进程中断，转而处理某一个突发事件。
◼ 发往进程的诸多信号，通常都是源于内核。引发内核为进程产生信号的各类事件如下：
    🎈 对于前台进程，用户可以通过输入特殊的终端字符来给它发送信号。比如输入Ctrl+ C 通常会给进程发送一个中断信号。
    🎈 硬件发生异常，即硬件检测到一个错误条件并通知内核，随即再由内核发送相应信号给相关进程。比如执行一条异常的机器语言指令，诸如被 0 除，
    或者引用了无法访问的内存区域。
    🎈 系统状态变化，比如 alarm 定时器到期将引起 SIGALRM 信号，进程执行的 CPU 时间超限，或者该进程的某个子进程退出。
    🎈 运行 kill 命令或调用 kill 函数。


2. 信号的5种处理动作
◼ 查看信号的详细信息：man 7 signal
◼ 信号的 5 中默认处理动作  Term, Ign, Core, Stop, Cont
    Term -> 终止进程
    Ign -> 当前进程忽略掉这个信号
    Core -> 终止进程，并生成一个Core文件
    Stop -> 暂停当前进程
    Cont -> 继续执行当前被暂停的进程
◼ 信号的几种状态：产生、未决、递达
◼ SIGKILL 和 SIGSTOP 信号不能被捕捉、阻塞或者忽略，只能执行默认动作。


3. 信号相关的函数
◼ int kill(pid_t pid, int sig);
◼ int raise(int sig);
◼ void abort(void);
◼ unsigned int alarm(unsigned int seconds);
◼ int setitimer(int which, const struct itimerval *new_val, struct itimerval *old_value);
◼ sighandler_t signal(int signum, sighandler_t handler);
◼ int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);
