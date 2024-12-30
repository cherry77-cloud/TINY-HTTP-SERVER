#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() 
{
    const char* serverIP = "127.0.0.1";
    const int serverPort = 5000;

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        std::cerr << "Failed to create socket.\n";
        return -1;
    }

    sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(serverPort);
    serverAddr.sin_addr.s_addr = inet_addr(serverIP);

    std::string message = "Hello from client!";
    ssize_t sentLen = sendto(sockfd, message.c_str(), message.size(), 0, (sockaddr*)&serverAddr, sizeof(serverAddr));
    if (sentLen < 0) {
        std::cerr << "sendto() error.\n";
        close(sockfd);
        return -1;
    }
    std::cout << "Message sent to server." << std::endl;

    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    sockaddr_in fromAddr;
    socklen_t fromAddrLen = sizeof(fromAddr);

    ssize_t recvLen = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0, (sockaddr*)&fromAddr, &fromAddrLen);
    if (recvLen < 0) {
        std::cerr << "recvfrom() error.\n";
        close(sockfd);
        return -1;
    }

    std::cout << "Received from server: " << buffer << std::endl;

    close(sockfd);
    return 0;
}
