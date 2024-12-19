1. 字节序转换函数
    uint16_t htons(uint16_t hostshort);
    uint32_t htonl(uint32_t hostlong);
    uint16_t ntohs(uint16_t netshort);
    uint32_t ntohl(uint32_t netlong);
无论主机字节序如何，网络字节序始终为大端序。因此，在发送数据到网络前，需将数据转换为大端序；接收数据后，需将其转换回主机字节序。


2. socket
    int socket(int domain, int type, int protocol);
◼ 创建一个新的套接字，用于网络通信。
◼ domain：指定通信域。常用值：
      AF_INET：IPv4互联网协议。
      AF_INET6：IPv6互联网协议。
      AF_UNIX：本地通信（同一主机内）。
◼ type：指定通信类型。
      SOCK_STREAM：提供有序、可靠、双向、基于连接的字节流（TCP）。
      SOCK_DGRAM：支持数据报（UDP）。
      SOCK_RAW：提供原始网络协议访问。
◼ protocol：指定要使用的具体协议。通常设置为 0，以选择给定 domain 和 type 的默认协议。


3. bind
    int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
◼ 将套接字绑定到特定的本地IP地址和端口号。
◼ sockfd：要绑定的套接字文件描述符。
◼ addr：指向包含地址信息的 sockaddr 结构体的指针。
◼ addrlen：addr 结构体的大小（字节数）。


4. listen
    int listen(int sockfd, int backlog);
◼ 将套接字标记为被动套接字，以便它可以接受传入的连接请求。
◼ sockfd：要监听的套接字文件描述符。
◼ backlog：挂起连接队列的最大长度。如果队列已满，新的连接请求可能会被拒绝。


5. accept
    int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
◼ 从挂起连接队列中提取第一个连接请求，创建一个新的已连接套接字，并返回新的套接字文件描述符。
◼ sockfd：监听套接字的文件描述符。
◼ addr：指向 sockaddr 结构体的指针，用于存储连接方的地址信息。
◼ addrlen：指向 socklen_t 变量的指针，初始时包含 addr 结构体的大小。返回时包含实际的地址大小。


6. connect
    int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
◼ 在套接字上发起与指定服务器的连接。
◼ sockfd：要连接的套接字文件描述符。
◼ addr：指向包含服务器地址信息的 sockaddr 结构体的指针。
◼ addrlen：addr 结构体的大小（字节数）。


7. send 和 recv
    ssize_t send(int sockfd, const void *buf, size_t len, int flags);
    ssize_t recv(int sockfd, void *buf, size_t len, int flags);
◼ 在已连接的套接字上发送数据。
◼ 从已连接的套接字上接收数据。


7. sendto 和 recvfrom
    ssize_t sendto(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen);
    ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen);
◼ 向特定目的地发送数据，主要用于数据报套接字（UDP）。
◼ 从套接字接收数据，并获取发送方的地址信息，主要用于数据报套接字（UDP）。


8. close
    int close(int fd);
◼ 关闭一个文件描述符，使其不再指向任何文件或套接字，并可能被系统重新使用


9. inet_pton 和 inet_ntop
    int inet_pton(int af, const char *src, void *dst);
    const char *inet_ntop(int af, const void *src, char *dst, socklen_t size);
◼ 将IP地址从文本格式转换为二进制格式。
◼ 将IP地址从二进制格式转换为文本格式。
