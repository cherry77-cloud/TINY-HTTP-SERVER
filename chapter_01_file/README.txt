1. 静态库的制作
◼ 库文件是计算机上的一类文件，可以简单的把库文件看成一种代码仓库，它提供给使用者一些可以直接拿来用的变量，函数或类。
◼ 库是特殊的一种程序，编写库的程序和编写一般的程序区别不大，只是库不能单独运行。
◼ 库文件有两种，静态库和动态库（共享库），区别是：静态库在程序的链接阶段被复制到了程序中；动态库在链接阶段没有被复制到程序中，
而是程序在运行时由系统动态加载到内存中供程序调用。
# 1. gcc 获得 .o 文件
# 2. 将 .o 文件打包, 使用 ar 工具
#    ar rcs libxxx.a xxx.o xxx.o


2. 动态库的制作
◼ 静态库：GCC 进行链接时，会把静态库中代码打包到可执行程序中
◼ 动态库：GCC 进行链接时，动态库的代码不会被打包到可执行程序中
◼ 程序启动之后，动态库会被动态加载到内存中，通过 ldd 命令检查动态库依赖关系
◼ 当系统加载可执行代码时候，能够知道其所依赖的库的名字，但是还需要知道绝对路径。此时就需要系统的动态载入器来获取该绝对路径。
对于elf格式的可执行程序，是由ld-linux.so来完成的，它先后搜索elf文件的 DT_RPATH段 -> 环境变量 LD_LIBRARY_PATH -> /etc/ld.so.cache文件列表 
-> /lib/，/usr/lib 目录找到库文件后将其载入内存。
# 动态库的制作
# gcc 得到 .o 文件, 得到位置无关的代码
    gcc -c -fpic/-fPIC a.c b.c
# gcc 得到 动态库
    gcc -shared a.o b.o -o libxxx.so


3. Makefile
文件命名
    makefile 或者 Makefile   
Makefile 规则
    ◼ 一个 Makefile 文件中可以有一个或者多个规则
        目标 ...: 依赖 ...
            命令（Shell 命令）
            ...
       目标：最终要生成的文件（伪目标除外）
       依赖：生成目标所需要的文件或是目标
       命令：通过执行命令对依赖操作生成目标（命令前必须 Tab 缩进）
    ◼ Makefile 中的其它规则一般都是为第一条规则服务的。

预定义变量
    AR : 归档维护程序的名称，默认值为 ar
    CC : C 编译器的名称，默认值为 cc
    CXX : C++ 编译器的名称，默认值为 g++
    $@ : 目标的完整名称
    $< : 第一个依赖文件的名称
    $^ : 所有的依赖文件


4. Linux 系统 IO 函数
◼ int open(const char *pathname, int flags);
◼ int open(const char *pathname, int flags, mode_t mode);
◼ int close(inf fd);
◼ ssize_t read(int fd, void *buf, size_t count);
◼ ssize_t write(int fd, const void *buf, size_t count);
◼ off_t lseek(int fd, off_t offset, int whence);
◼ int stat(const char *pathname, struct stat *statbuf);
◼ int lstat(const char *pathname, struct stat *statbuf);

🍞 int access(const char *pathname, int mode);
🍞 int chmod(const char *filename, int mode);
🍞 int chown(const char *path, uid_t owner, gid_t group);
🍞 int truncate(const char *path, off_t length);

🍕 int rename(const char *oldpath, const char *newpath);
🍕 int chdir(const char *path);
🍕 char *getcwd(char *buf, size_t size);
🍕 int mkdir(const char *pathname, mode_t mode);
🍕 int rmdir(const char *pathname);
🍕 DIR *opendir(const char *name);
🍕 struct dirent *readdir(DIR *dirp);
🍕 int closedir(DIR *dirp);

// stat 结构体
struct stat {
    dev_t st_dev;            // 文件的设备编号
    ino_t st_ino;            // 节点
    mode_t st_mode;          // 文件的类型和存取的权限
    nlink_t st_nlink;        // 连到该文件的硬连接数目
    uid_t st_uid;            // 用户 ID
    gid_t st_gid;            // 组 ID
    dev_t st_rdev;           // 设备文件的设备编号
    off_t st_size;           // 文件字节数
    blksize_t st_blksize;    // 块大小
    blkcnt_t st_blocks;      // 块数
    time_t st_atime;         // 最后一次访问时间
    time_t st_mtime;         // 最后一次修改时间
    time_t st_ctime;         // 最后一次改变时间(指属性)
};

struct dirent {
    ino_t d_ino;
    off_t d_off;
    unsigned short int d_reclen;
    unsigned char d_type;
    char d_name[256];
};

# DT_BLK - 块设备    DT_CHR - 字符设备   DT_DIR - 目录       DT_LNK - 软连接
# DT_REG - 普通文件  DT_SOCK - 套接字    DT_UNKNOWN - 未知   DT_FIFO - 管道
