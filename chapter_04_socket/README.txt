1. 字节序转换函数
#include <arpa/inet.h>
uint16_t htons(uint16_t hostshort);
uint32_t htonl(uint32_t hostlong);
uint16_t ntohs(uint16_t netshort);
uint32_t ntohl(uint32_t netlong);
无论主机字节序如何，网络字节序始终为大端序。因此，在发送数据到网络前，需将数据转换为大端序；接收数据后，需将其转换回主机字节序。
