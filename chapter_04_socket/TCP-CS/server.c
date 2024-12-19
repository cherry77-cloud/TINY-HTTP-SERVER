#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>

#define PORT 39696
#define BUFFER_SIZE 1024
#define BACKLOG 10

int main() 
{
    // Create a TCP socket
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Set SO_REUSEADDR option to reuse the address
    int opt = 1;
    if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
        perror("setsockopt");
        close(listen_fd);
        exit(EXIT_FAILURE);
    }

    // Configure server address structure
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT); // Convert port to network byte order
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Bind to loopback address

    // Bind the socket to the specified IP and port
    if (bind(listen_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        close(listen_fd);
        exit(EXIT_FAILURE);
    }

    // Start listening for incoming connections
    if (listen(listen_fd, BACKLOG) == -1) {
        perror("listen");
        close(listen_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on 127.0.0.1:%d...\n", PORT);

    while (1) {
        // Accept a new client connection
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &client_len);
        if (client_fd == -1) {
            perror("accept");
            continue; // Continue to accept new connections
        }

        // Get client's IP address and port
        char client_ip[INET_ADDRSTRLEN];
        if (inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip)) == NULL) {
            perror("inet_ntop");
            close(client_fd);
            continue;
        }
        unsigned short client_port = ntohs(client_addr.sin_port);
        printf("Connected to client %s:%d\n", client_ip, client_port);

        // Handle client communication
        char buffer[BUFFER_SIZE];
        while (1) {
            ssize_t bytes_read = read(client_fd, buffer, sizeof(buffer));
            if (bytes_read == -1) {
                perror("read");
                break; // Exit the client handling loop
            } else if (bytes_read > 0) {
                // Null-terminate the received data
                buffer[bytes_read] = '\0';
                printf("Received from %s:%d: %s\n", client_ip, client_port, buffer);

                // Echo the received data back to the client
                ssize_t bytes_written = 0;
                ssize_t total_written = 0;
                while (total_written < bytes_read) {
                    bytes_written = write(client_fd, buffer + total_written, bytes_read - total_written);
                    if (bytes_written == -1) {
                        perror("write");
                        break;
                    }
                    total_written += bytes_written;
                }
                if (bytes_written == -1) {
                    break; // Exit the client handling loop on write error
                }
            } else if (bytes_read == 0) {
                printf("Client %s:%d disconnected.\n", client_ip, client_port);
                break; // Client has closed the connection
            }
        }
        // Close the client socket
        close(client_fd);
    }
    // Close the listening socket (unreachable code in this example)
    close(listen_fd);
    return 0;
}
