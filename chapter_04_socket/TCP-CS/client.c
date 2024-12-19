#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define PORT 39696
#define BUFFER_SIZE 1024

int main()
{
    // Create a TCP socket
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    // Configure server address structure
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT); // Convert port to network byte order
    // Convert IP address from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        perror("inet_pton");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }
    // Connect to the server
    if (connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }
    printf("Connected to server at 127.0.0.1:%d\n", PORT);
    char send_buffer[BUFFER_SIZE];
    char recv_buffer[BUFFER_SIZE];
    while (1) {
        printf("Enter message (or type 'exit' to quit): ");
        // Read input from the user
        if (fgets(send_buffer, sizeof(send_buffer), stdin) == NULL) {
            printf("\nInput terminated. Exiting.\n");
            break;
        }
        // Remove the trailing newline character
        size_t len = strlen(send_buffer);
        if (len > 0 && send_buffer[len - 1] == '\n') {
            send_buffer[len - 1] = '\0';
            len--;
        }
        // Check for exit command
        if (strcmp(send_buffer, "exit") == 0) {
            printf("Exiting client.\n");
            break;
        }
        // Send the message to the server
        ssize_t bytes_sent = write(sock_fd, send_buffer, len);
        if (bytes_sent == -1) {
            perror("write");
            break;
        }
        // Receive the echo from the server
        ssize_t bytes_received = read(sock_fd, recv_buffer, sizeof(recv_buffer) - 1);
        if (bytes_received == -1) {
            perror("read");
            break;
        } else if (bytes_received == 0) {
            printf("Server closed the connection.\n");
            break;
        }
        // Null-terminate and print the received message
        recv_buffer[bytes_received] = '\0';
        printf("Echo from server: %s\n", recv_buffer);
    }

    // Close the socket
    close(sock_fd);
    return 0;
}
