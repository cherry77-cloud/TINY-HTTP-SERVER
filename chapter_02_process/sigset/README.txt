1. 信号集
◼ 许多信号相关的系统调用都需要能表示一组不同的信号，多个信号可使用一个称之为信号集的数据结构来表示，其系统数据类型为 sigset_t。
◼ 在 PCB 中有两个非常重要的信号集。一个称之为 “阻塞信号集” ，另一个称之为 “未决信号集” 。这两个信号集都是内核使用位图机制来实现的。
但操作系统不允许我们直接对这两个信号集进行位操作。而需自定义另外一个集合，借助信号集操作函数来对 PCB 中的这两个信号集进行修改。
◼ 信号的 “未决” 是一种状态，指的是从信号的产生到信号被处理前的这一段时间。
◼ 信号的 “阻塞” 是一个开关动作，指的是阻止信号被处理，但不是阻止信号产生。
◼ 信号的阻塞就是让系统暂时保留信号留待以后发送。由于另外有办法让系统忽略信号，所以一般情况下信号的阻塞只是暂时的，只是为了防止信号打断敏感的操作。


2. 信号集操作函数
◼ int sigemptyset(sigset_t *set);
◼ int sigfillset(sigset_t *set);
◼ int sigaddset(sigset_t *set, int signum);
◼ int sigdelset(sigset_t *set, int signum);
◼ int sigismember(const sigset_t *set, int signum);
◼ int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
◼ int sigpending(sigset_t *set);


3. SIGCHLD信号
◼ SIGCHLD信号产生的条件
    🎈 子进程终止时    🎈 子进程接收到 SIGSTOP 信号停止时    🎈 子进程处在停止态，接受到SIGCONT后唤醒时
◼ 以上三种条件都会给父进程发送 SIGCHLD 信号，父进程默认会忽略该信号
