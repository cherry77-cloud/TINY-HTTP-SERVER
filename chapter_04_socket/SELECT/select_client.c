#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

#define SERVER_PORT 9999
#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) 
{
    const char *server_ip = "127.0.0.1";
    if (argc > 1) {
        server_ip = argv[1];
    }

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port   = htons(SERVER_PORT);

    if (inet_pton(AF_INET, server_ip, &serv_addr.sin_addr) <= 0) {
        perror("inet_pton");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connect");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("Connected to server %s:%d\n", server_ip, SERVER_PORT);

    while (1) {
        char send_buf[BUFFER_SIZE] = {0};
        if (fgets(send_buf, sizeof(send_buf), stdin) == NULL) {
            printf("EOF reached, client exit...\n");
            break;
        }
        ssize_t n = write(sockfd, send_buf, strlen(send_buf));
        if (n < 0) {
            perror("write");
            break;
        }
        char recv_buf[BUFFER_SIZE] = {0};
        n = read(sockfd, recv_buf, sizeof(recv_buf));
        if (n < 0) {
            perror("read");
            break;
        } else if (n == 0) {
            printf("Server closed connection, client exit...\n");
            break;
        }
        printf("Server: %s\n", recv_buf);
    }
    close(sockfd);
    return 0;
}
