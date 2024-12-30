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
    serverAddr.sin_addr.s_addr = inet_addr(serverIP);
    serverAddr.sin_port = htons(serverPort);

    if (bind(sockfd, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Failed to bind socket.\n";
        close(sockfd);
        return -1;
    }

    std::cout << "UDP server started on " << serverIP << ":" << serverPort << std::endl;

    while (true) {
        char buffer[1024];
        memset(buffer, 0, sizeof(buffer));
        sockaddr_in clientAddr;
        socklen_t clientAddrLen = sizeof(clientAddr);

        ssize_t recvLen = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0, (sockaddr*)&clientAddr, &clientAddrLen);
        if (recvLen < 0) {
            std::cerr << "recvfrom() error.\n";
            continue;
        }

        std::cout << "Received message: " << buffer 
                  << " from " << inet_ntoa(clientAddr.sin_addr)
                  << ":" << ntohs(clientAddr.sin_port) << std::endl;

        std::string reply = "Hello from server!";
        ssize_t sentLen = sendto(sockfd, reply.c_str(), reply.size(), 0,
                                 (sockaddr*)&clientAddr, clientAddrLen);
        if (sentLen < 0) {
            std::cerr << "sendto() error.\n";
        }
    }

    close(sockfd);
    return 0;
}
