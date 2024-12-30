#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>

#define SERVER_PORT 9999
#define LISTEN_BACKLOG 8

int main() 
{
    int lfd = socket(PF_INET, SOCK_STREAM, 0);
    if (lfd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    int opt = 1;
    if (setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt");
        close(lfd);
        exit(EXIT_FAILURE);
    }
    struct sockaddr_in saddr;
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family      = AF_INET;
    saddr.sin_port        = htons(SERVER_PORT);
    saddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(lfd, (struct sockaddr *)&saddr, sizeof(saddr)) < 0) {
        perror("bind");
        close(lfd);
        exit(EXIT_FAILURE);
    }
    if (listen(lfd, LISTEN_BACKLOG) < 0) {
        perror("listen");
        close(lfd);
        exit(EXIT_FAILURE);
    }
    printf("Server is listening on port %d...\n", SERVER_PORT);
  
    fd_set rdset;
    fd_set tmp;
    FD_ZERO(&rdset);
    FD_SET(lfd, &rdset);
    int maxfd = lfd;
    while (1) {
        tmp = rdset;
        int ret = select(maxfd + 1, &tmp, NULL, NULL, NULL);
        if (ret < 0) {
            if (errno == EINTR) {
                continue;
            }
            perror("select");
            exit(EXIT_FAILURE);
        }
        if (FD_ISSET(lfd, &tmp)) {
            struct sockaddr_in cliaddr;
            socklen_t len = sizeof(cliaddr);
            int cfd = accept(lfd, (struct sockaddr *)&cliaddr, &len);
            if (cfd < 0) {
                perror("accept");
                continue;
            }
            char ip[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &cliaddr.sin_addr, ip, sizeof(ip));
            printf("New client connected: %s:%d (fd=%d)\n", 
                   ip, ntohs(cliaddr.sin_port), cfd);
            FD_SET(cfd, &rdset);
            if (cfd > maxfd) {
                maxfd = cfd;
            }
            if (--ret <= 0) {
                continue;
            }
        }

        for (int i = lfd + 1; i <= maxfd; i++) {
            if (FD_ISSET(i, &tmp)) {
                char buf[1024] = {0};
                int len = read(i, buf, sizeof(buf));
                if (len < 0) {
                    perror("read");
                    close(i);
                    FD_CLR(i, &rdset);
                } else if (len == 0) {
                    printf("client (fd=%d) closed...\n", i);
                    close(i);
                    FD_CLR(i, &rdset);
                } else {
                    printf("Received from client(fd=%d): %s\n", i, buf);
                    if (write(i, buf, len) < 0) {
                        perror("write");
                        close(i);
                        FD_CLR(i, &rdset);
                    }
                }
                if (--ret <= 0) {
                    break;
                }
            }
        }
    }
    close(lfd);
    return 0;
}
