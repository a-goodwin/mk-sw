#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <strings.h>
#include <onion-i2c.h>
static unsigned char sendBuf[1025];

int main(int argc, char **argv, char **envp)
{
    int addr;
    int size;
    //int i;
    int ret;
    unsigned char v;
    char* st;
    char* sti;
    if (argc<2) {
        printf("I2C raw read routine\r\n");
        printf("Usage: %s <devAddr> <data>\r\n reads <size> bytes from dev <devAddr>\r\nexample: %s 0x6e f5:ff:ff:00:00:7e:00", argv[0], argv[0]);
        return 1;
    }
    if (strstr(argv[1], "0x")==NULL) {
        addr = strtol(argv[1], NULL, 10);
    } else addr = strtol(argv[1]+2, NULL, 16);

    st = argv[2];
    sti = strtok(st, ":");
    size = 0;

    while (sti!=NULL) {
        v = strtol(sti, NULL, 16);
        sendBuf[size++]=v;
        printf("%02x:", v);
        sti = strtok(NULL, ":");
    }
    printf("\r\n");

    if (size==0) {
        printf("empty data\r\n");
        return 1;
    }

    ret = i2c_writeBufferRaw(0, addr, sendBuf, size);
    printf("write %i bytes to dev 0x%02x\r\n", size, addr);
    return 0;
}
