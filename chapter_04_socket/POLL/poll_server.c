#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <poll.h>
#include <errno.h>

#define MAX_CLIENTS 1024
#define PORT 9999
#define BUFFER_SIZE 1024

int main() 
{
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    if (lfd < 0) { perror("socket"); exit(EXIT_FAILURE); }

    int opt = 1;
    if (setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt");
        close(lfd);
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in saddr = {0};
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = INADDR_ANY;
    saddr.sin_port = htons(PORT);

    if (bind(lfd, (struct sockaddr*)&saddr, sizeof(saddr)) < 0) {
        perror("bind");
        close(lfd);
        exit(EXIT_FAILURE);
    }

    if (listen(lfd, 8) < 0) {
        perror("listen");
        close(lfd);
        exit(EXIT_FAILURE);
    }

    struct pollfd fds[MAX_CLIENTS];
    memset(fds, -1, sizeof(fds));
    fds[0].fd = lfd;
    fds[0].events = POLLIN;
    int nfds = 1;

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        int ready = poll(fds, nfds, -1);
        if (ready < 0) {
            if (errno == EINTR) continue;
            perror("poll");
            break;
        }

        if (fds[0].revents & POLLIN) {
            struct sockaddr_in cliaddr;
            socklen_t len = sizeof(cliaddr);
            int cfd = accept(lfd, (struct sockaddr*)&cliaddr, &len);
            if (cfd < 0) { perror("accept"); continue; }
            printf("New connection: %s:%d, fd=%d\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port), cfd);
            int i;
            for (i = 1; i < MAX_CLIENTS; i++) {
                if (fds[i].fd == -1) {
                    fds[i].fd = cfd;
                    fds[i].events = POLLIN;
                    if (i >= nfds) nfds = i + 1;
                    break;
                }
            }
            if (i == MAX_CLIENTS) { printf("Max clients reached.\n"); close(cfd); }
            if (--ready <= 0) continue;
        }

        for (int i = 1; i < nfds && ready > 0; i++) {
            if (fds[i].fd == -1) continue;
            if (fds[i].revents & (POLLIN | POLLERR | POLLHUP)) {
                char buf[BUFFER_SIZE];
                ssize_t n = recv(fds[i].fd, buf, sizeof(buf), 0);
                if (n > 0) {
                    send(fds[i].fd, buf, n, 0);
                } else {
                    printf("Client fd=%d disconnected.\n", fds[i].fd);
                    close(fds[i].fd);
                    fds[i].fd = -1;
                    if (i == nfds - 1) {
                        while (nfds > 1 && fds[nfds-1].fd == -1) nfds--;
                    }
                }
                ready--;
            }
        }
    }

    for (int i = 0; i < nfds; i++) if (fds[i].fd != -1) close(fds[i].fd);
    return 0;
}
