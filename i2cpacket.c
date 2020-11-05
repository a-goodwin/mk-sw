#include "i2cpacket.h"
#include <onion-i2c.h>
//#include <stdio.h>
#include <sys/socket.h>
#include "globals.h"
#include "sock.h"

static int devAdresses[C_KT_COUNT] = {0x6e, 0x70, 0x72, 0x74};

//extern int connfd[C_KT_COUNT];

#define BUF_SZ 256
//xstatic unsigned char obuf[BUF_SZ]; // eth to i2c stream buffer
static unsigned char ibuf[BUF_SZ]; // i2c to eth stream buffer
static int psz=0;

void i2c_poll(void)
{
    int i=0;
    int addr;
    //int ret;
    for (i=0; i<C_KT_COUNT; i++) {
        addr = devAdresses[i];
        // poll that i2c has data
        memset(ibuf, 0, C_PKT_HDR_SZ);
        psz = devGetPacket(addr, ibuf);
        if (psz) {
            printf("i2c%i x%02x: has %i data\r\n", i, addr, psz);
            printPacket(ibuf);
            sock_send(i, ibuf, (size_t)psz);
            break; // if there's a packet in one stm,
                   //no check other stms for packet in that loop!
        } else     {};//printf(".\r\n");
    }
}

// iterative function
int devGetPacket(int devId, unsigned char *bufptr)
{
    int ret;
    int size;
    unsigned char *psz = bufptr+4;
    ret = i2c_readRaw(0, devId, bufptr, C_PKT_HDR_SZ);
    if (bufptr[0]!=0xff && bufptr[0]!=0xf5 && bufptr[0]!=0x5f && bufptr[0]!=0xf6) {
        return 0;
    }
    size = C_PKT_HDR_SZ;
    if (*psz!=0) { // read data and dcrc
        ret |= i2c_readRaw(0, devId, bufptr+size, (*psz)+1);
        size += (*psz)+1;
    } else {
        ret |= i2c_readRaw(0, devId, bufptr+size, (*psz)+1);
        size += (*psz)+1;
    };
    if (ret==EXIT_SUCCESS) return size; else return 0;
}

int devSendPacket(int devId, unsigned char *buf, int size)
{
    int ret;
    int addr = devAdresses[devId];

    printf("i2c_wBuf %i: %i\r\n", devId, size);
    ret = i2c_writeBufferRaw(0, addr, buf, size);
    return ret;
}

#pragma pack(push, 1)
void printPacket(unsigned char *buf)
{
    unsigned char *psz = buf+4;
    unsigned short addr = buf[1] << 8 | buf[2];
    int i=0;
    if (*(buf)!=0xf5 && *(buf)!=0x5f && *(buf)!=0xf7 && *(buf)!=0xf6) {
        printf("invalid sign: 0x%02x\r\n", buf[0]);
        return;
    }
    if (*(buf)!=0xf7 && *(buf)!=0xf6) {
        printf("pkt 0x%02x @0x%04x sz 0x%02x hcrc 0x%02x dcrc 0x%02x",
               *(buf+3),
               addr,
               *psz,
               *(buf+5), *(buf+6+*psz));
        if ((*psz)!=0) {
            for (i=0; i<(*psz); i++) printf("%02X ", *(buf+6+i));
        }
        printf("\r\n");
        return;
    }
}
#pragma pack(pop)

void _i2c_read(int devId, unsigned char *buffer, int reqBytes)
{
    int ret;
    ret = i2c_readRaw(0, devId, buffer, reqBytes);

}
