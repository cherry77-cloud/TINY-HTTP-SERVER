#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 9999
#define BUFFER_SIZE 1024

int main() 
{
    int cfd = socket(AF_INET, SOCK_STREAM, 0);
    if (cfd < 0) { perror("socket"); exit(EXIT_FAILURE); }

    struct sockaddr_in saddr = {0};
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &saddr.sin_addr) <= 0) {
        perror("inet_pton");
        close(cfd);
        exit(EXIT_FAILURE);
    }

    if (connect(cfd, (struct sockaddr*)&saddr, sizeof(saddr)) < 0) {
        perror("connect");
        close(cfd);
        exit(EXIT_FAILURE);
    }

    printf("Connected to %s:%d\n", SERVER_IP, SERVER_PORT);

    char send_buf[BUFFER_SIZE], recv_buf[BUFFER_SIZE];
    while (1) {
        printf("Message: ");
        if (!fgets(send_buf, sizeof(send_buf), stdin)) break;

        size_t len = strlen(send_buf);
        if (send_buf[len-1] == '\n') send_buf[--len] = '\0';

        if (strcmp(send_buf, "exit") == 0) break;

        if (send(cfd, send_buf, len, 0) < 0) { perror("send"); break; }

        ssize_t n = recv(cfd, recv_buf, sizeof(recv_buf)-1, 0);
        if (n > 0) {
            recv_buf[n] = '\0';
            printf("Echo: %s\n", recv_buf);
        } else if (n == 0) {
            printf("Server closed connection.\n");
            break;
        } else {
            perror("recv");
            break;
        }
    }

    close(cfd);
    return 0;
}
