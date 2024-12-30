#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <signal.h>

#define SERVER_PORT 9999
#define MAX_CLIENTS 128
#define BUFFER_SIZE 1024

struct sockInfo {
    int fd;
    struct sockaddr_in addr;
    pthread_t tid;
};

struct sockInfo sockinfos[MAX_CLIENTS];
pthread_mutex_t sockinfos_mutex = PTHREAD_MUTEX_INITIALIZER;

void *handle_client(void *arg) 
{
    struct sockInfo *pinfo = (struct sockInfo *)arg;
    char cliIp[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(pinfo->addr.sin_addr), cliIp, INET_ADDRSTRLEN);
    unsigned short cliPort = ntohs(pinfo->addr.sin_port);
    printf("Client connected: %s:%d\n", cliIp, cliPort);

    char recvBuf[BUFFER_SIZE];
    while(1) {
        memset(recvBuf, 0, sizeof(recvBuf));
        ssize_t len = recv(pinfo->fd, recvBuf, sizeof(recvBuf) - 1, 0);
        if(len == -1) {
            perror("recv");
            break;
        } else if(len > 0) {
            printf("Received from %s:%d - %s\n", cliIp, cliPort, recvBuf);
            ssize_t sent = send(pinfo->fd, recvBuf, len, 0);
            if(sent == -1) {
                perror("send");
                break;
            }
        } else if(len == 0) {
            printf("Client %s:%d disconnected.\n", cliIp, cliPort);
            break;
        }
    }
    close(pinfo->fd);
    pthread_mutex_lock(&sockinfos_mutex);
    pinfo->fd = -1;
    pinfo->tid = 0;
    pthread_mutex_unlock(&sockinfos_mutex);
    return NULL;
}

int main() 
{
    signal(SIGPIPE, SIG_IGN);
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    if (lfd == -1){
        perror("socket");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    if (setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1){
        perror("setsockopt");
        close(lfd);
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in saddr;
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(SERVER_PORT);
    saddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(lfd, (struct sockaddr *)&saddr, sizeof(saddr)) == -1) {
        perror("bind");
        close(lfd);
        exit(EXIT_FAILURE);
    }
    if (listen(lfd, MAX_CLIENTS) == -1) {
        perror("listen");
        close(lfd);
        exit(EXIT_FAILURE);
    }

    printf("Server started on port %d. Waiting for connections...\n", SERVER_PORT);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        sockinfos[i].fd = -1;
        sockinfos[i].tid = 0;
    }

    while(1) {
        struct sockaddr_in cliaddr;
        socklen_t len = sizeof(cliaddr);
        int cfd = accept(lfd, (struct sockaddr*)&cliaddr, &len);
        if (cfd == -1) {
            perror("accept");
            continue;
        }
        int i;
        for (i = 0; i < MAX_CLIENTS; i++) {
            if(sockinfos[i].fd == -1) {
                sockinfos[i].fd = cfd;
                memcpy(&sockinfos[i].addr, &cliaddr, sizeof(cliaddr));
                break;
            }
        }
        if (i == MAX_CLIENTS) {
            printf("Maximum clients connected. Connection rejected.\n");
            close(cfd);
            continue;
        }
        if (pthread_create(&sockinfos[i].tid, NULL, handle_client, (void *)&sockinfos[i]) != 0) {
            perror("pthread_create");
            close(cfd);
            pthread_mutex_lock(&sockinfos_mutex);
            sockinfos[i].fd = -1;
            sockinfos[i].tid = 0;
            pthread_mutex_unlock(&sockinfos_mutex);
            continue;
        }
        pthread_detach(sockinfos[i].tid);
    }
    close(lfd);
    return 0;
}
