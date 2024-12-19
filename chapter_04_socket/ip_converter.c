#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

int main() 
{
    // IP address in dotted decimal notation
    const char *ip_str = "192.168.1.4";
    struct in_addr addr;

    // Convert IP string to network byte order binary form
    int result = inet_pton(AF_INET, ip_str, &addr);
    if (result == 0) {
        fprintf(stderr, "Invalid IP address format: %s\n", ip_str);
        return EXIT_FAILURE;
    } else if (result < 0) {
        perror("inet_pton");
        return EXIT_FAILURE;
    }

    // Access each byte of the IP address
    unsigned char *bytes = (unsigned char *)&addr.s_addr;
    printf("IP bytes: %d %d %d %d\n", bytes[0], bytes[1], bytes[2], bytes[3]);

    // Convert network byte order binary form back to IP string
    char converted_ip[INET_ADDRSTRLEN];
    const char *str = inet_ntop(AF_INET, &addr, converted_ip, sizeof(converted_ip));
    if (str == NULL) {
        perror("inet_ntop");
        return EXIT_FAILURE;
    }

    printf("Converted IP: %s\n", str);
    printf("Pointers equal: %s\n", (converted_ip == str) ? "Yes" : "No");

    return 0;
}
