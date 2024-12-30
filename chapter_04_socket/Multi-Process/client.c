#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>

// Define server IP and port
#define SERVER_IP   "127.0.0.1"
#define SERVER_PORT 5000

// Flag to control the loop
volatile sig_atomic_t keep_running = 1;

// Signal handler to gracefully exit on Ctrl+C
void int_handler(int dummy) 
{
    keep_running = 0;
}

int main() 
{
    // Register signal handler for graceful termination
    signal(SIGINT, int_handler);

    // Create a TCP socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return -1;
    }

    // Define server address
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        close(sockfd);
        return -1;
    }

    printf("Connected to server %s:%d\n", SERVER_IP, SERVER_PORT);

    // Enter a loop to send messages to the server
    while (keep_running) {
        char message[1024];
        printf("Enter message to send (or 'exit' to quit): ");
        if (fgets(message, sizeof(message), stdin) == NULL) {
            perror("fgets");
            break;
        }

        // Remove newline character
        message[strcspn(message, "\n")] = '\0';

        // Check for exit condition
        if (strcmp(message, "exit") == 0) {
            printf("Exiting client.\n");
            break;
        }

        // Send the message to the server
        if (send(sockfd, message, strlen(message), 0) == -1) {
            perror("send");
            break;
        }

        // Receive response from the server
        char buffer[1024];
        memset(buffer, 0, sizeof(buffer));
        ssize_t bytes_received = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received < 0) {
            perror("recv");
            break;
        }
        else if (bytes_received == 0) {
            printf("Server closed the connection.\n");
            break;
        }

        printf("Received from server: %s\n", buffer);
    }

    // Close the socket
    close(sockfd);
    return 0;
}
