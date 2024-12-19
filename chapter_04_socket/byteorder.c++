#include <iostream>
#include <cstdint>

int main() 
{
    uint32_t x = 0x12345678;
    uint8_t *c = reinterpret_cast<uint8_t*>(&x);
    std::cout << "Memory representation of 0x12345678:" << std::endl;
    for (int i = 0; i < sizeof(x); ++i) {
        printf("Byte %d: 0x%02X\n", i, c[i]);
    }
    if (c[0] == 0x12) {
        std::cout << "This machine is Big Endian." << std::endl;
    } else if (c[0] == 0x78) {
        std::cout << "This machine is Little Endian." << std::endl;
    } else {
        std::cout << "Unknown Endianness." << std::endl;
    }
    return 0;
}
