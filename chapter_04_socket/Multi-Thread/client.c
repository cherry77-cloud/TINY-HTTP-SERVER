#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 9999
#define BUFFER_SIZE 1024

volatile sig_atomic_t keep_running = 1;

void int_handler(int dummy) 
{
    keep_running = 0;
}

int main() 
{
    signal(SIGINT, int_handler);
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1){
        perror("socket");
        exit(EXIT_FAILURE);
    }
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("inet_pton");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("Connected to server %s:%d\n", SERVER_IP, SERVER_PORT);

    while (keep_running) {
        char sendBuf[BUFFER_SIZE];
        printf("Enter message to send (type 'exit' to quit): ");
        if (fgets(sendBuf, sizeof(sendBuf), stdin) == NULL) {
            perror("fgets");
            break;
        }
        sendBuf[strcspn(sendBuf, "\n")] = '\0';
        if (strcmp(sendBuf, "exit") == 0) {
            printf("Exiting client.\n");
            break;
        }
        ssize_t sent = send(sockfd, sendBuf, strlen(sendBuf), 0);
        if (sent == -1) {
            perror("send");
            break;
        }
        char recvBuf[BUFFER_SIZE];
        memset(recvBuf, 0, sizeof(recvBuf));
        ssize_t len = recv(sockfd, recvBuf, sizeof(recvBuf) - 1, 0);
        if (len == -1) {
            perror("recv");
            break;
        } else if (len == 0) {
            printf("Server closed the connection.\n");
            break;
        }
        printf("Received from server: %s\n", recvBuf);
    }
    close(sockfd);
    return 0;
}
