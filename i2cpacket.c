#include "i2cpacket.h"
#include <onion-i2c.h>
//#include <stdio.h>
#include <sys/socket.h>
#include "globals.h"

static int devAdresses[C_KT_COUNT] = {0x68};

extern int connfd[C_KT_COUNT];

#define OBUF_SZ 256
static unsigned char obuf[OBUF_SZ];
static int psz=0;

void i2c_poll(void)
{
    int i=0;
    int addr;
    int sz;
    //int ret;
    for (i=0; i<C_KT_COUNT; i++) {
        addr = devAdresses[i];
        // poll that i2c has data
        sz = devHasData(addr);
        if (sz) {
            printf("i2c%i x%02x: has %i data\r\n", i, addr, sz);
            //psz = sz;
            psz = devGetPacket(addr, obuf, sz);
            obuf[psz]='\0';
            printf("i2c%i x%02x data: (%s)\r\n", i, addr, obuf);
            if (connfd[i]>0) write(connfd[i], obuf, (size_t)psz);
        }
    }
}


int devHasData(int devId)
{
    int ret=0;
    int v=0;
    printf("i2c_rb 0\r\n");
    ret = i2c_readByte(0, devId, I2C_REG_RX_SIZE, &v);
    if (ret==EXIT_SUCCESS) return v; else return 0;
}

int devGetPacket(int devId, unsigned char *bufptr, int size)
{
    int ret;
    printf("i2c_r 1 %i\r\n", size);
    ret = i2c_read(0, devId, I2C_REG_RX_DATA, bufptr, size);
    if (ret==EXIT_SUCCESS) return size; else return 0;
}

int devSendPacket(int devId, unsigned char *buf, int size)
{
    int ret;
    int txSz=0;
    printf("i2c_rb 2\r\n");
    ret = i2c_readByte(0, devId, I2C_REG_TX_SIZE, &txSz);
    if (ret==EXIT_FAILURE) return EXIT_FAILURE;
    else if (txSz) return EXIT_FAILURE;
    printf("i2c_wB 3\r\n");
    ret = i2c_writeBuffer(0, devId, 3, buf, size);
    return ret;
}
