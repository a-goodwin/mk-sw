#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <strings.h>
#include <onion-i2c.h>
static char recvBuf[1025];

int main(int argc, char **argv, char **envp)
{
    int addr;
    int size;
    int i;
    if (argc<2) {
        printf("I2C raw read routine\r\n");
        printf("Usage: %s <devAddr> <size>\r\n reads <size> bytes from dev <devAddr>\r\n", argv[0]);
        return 1;
    }
    size = atoi(argv[2]);
    if (strstr(argv[1], "0x")==NULL) {
        addr = strtol(argv[1], NULL, 10);
    } else addr = strtol(argv[1]+2, NULL, 16);

    printf("read %i bytes from dev 0x%02x\r\n", size, addr);
    int ret;
    ret = i2c_readRaw(0, addr, recvBuf, size);

    for(i=0; i<size; i++) {
           printf("%02X ", (unsigned char)recvBuf[i]);
    }
    printf("\r\n");

    return 0;

}
