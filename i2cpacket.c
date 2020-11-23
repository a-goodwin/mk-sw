#include "i2cpacket.h"
#include <onion-i2c.h>
#include <stdio.h>
#include <sys/socket.h>
#include "globals.h"
#include "sock.h"
#include "cmdpacket.h"
#include "gpio18.h"

//#include "stdlib.h"
#include "ctime.h"

//std::time_t;
static int devAdresses[C_KT_COUNT] = {0x32, 0x34, 0x36, 0x38};
static unsigned char cpingu[7] = {0xf5, 0xff, 0xff, 0x00, 0x00, 0xff, 0x00};
//extern int connfd[C_KT_COUNT];
//xstatic unsigned char obuf[BUF_SZ]; // eth to i2c stream buffer
static unsigned char ibuf[BUF_SZ+1]; // i2c to eth packet buffer
//static unsigned char pbuf[BUF_SZ+1]; // eth to i2c stream buffer

static tCmdBuf i2cbuf[C_KT_COUNT];




static int psz=0;
void i2c_init(void)
{
    int i=0;
    for (int i=0; i<C_KT_COUNT; i++) {
        bufclear(&i2cbuf[i]);
    }
}

void i2c_poll(void)
{
    int i=0;
    int addr;
    //int ret;
    if (!gpio_poll()) return;
    for (i=0; i<C_KT_COUNT; i++) {
        if (!gpio_poll()) break;
        addr = devAdresses[i];
        // poll that i2c has data
        memset(ibuf, 0, C_FULL_PACKET_SZ+1);
        psz = devGetPacket(i, addr, ibuf);
        if (psz) {
            printf(CLKHD " i2c%i 0x%02x: ", getms(), i, addr);
            printhex("", ibuf, psz+1);//printPacket(ibuf+1);
            //if (psz==2) { // shitty F6:F6 packet
            //    printf(CLKHD " ping (%i)->\r\n", getms(), i);
            //    devSendPacket(i, cpingu, 7); // send ping
            //} else {
                sock_send(i, ibuf+1, (size_t)psz);
            //}
            //break; // if there's a packet in one stm,
                   //no check other stms for packet in that loop!
        } else     {};//printf(".\r\n");
    }
}

// iterative function
int devGetPacket(int devnum, int devId, unsigned char *bufptr)
{
    int ret;
    tCmdBuf *cmd;
    unsigned char sz=0;
    //int szreal;
    ////// full buf read
    ret = i2c_readRaw(0, devId, &sz, 1);
    if (ret!=EXIT_SUCCESS) return 0;
    if (sz==0) return 0;
    // non zero packet length
    ret = i2c_readRaw(0, devId, bufptr, sz+1);
    if (ret!=EXIT_SUCCESS) return 0;
    return sz;

    ////// queue read - now disabled
#if(0)
    // TODO: read one byte: size
    ret = i2c_readRaw(0, devId, &sz, 1);
    if (ret!=EXIT_SUCCESS) return 0;
    if (sz==0) return 0;
    // TODO: read (size) bytes
    ret = i2c_readRaw(0, devId, pbuf, sz+1);
    if (pbuf[0]==0) {
        printf("i2c %i error in size byte: %i", devnum, pbuf[0]);
        return 0;
    }
    printf("i2c %i ", devnum);
    printhex("data", (unsigned char*)pbuf, (int)sz+1);
    if (ret!=EXIT_SUCCESS) return 0;

    ret = processPacketData(&i2cbuf[devnum], pbuf+1, sz);
    if (ret>0) { // command: copy data and return it
        cmd = getCmd();
        memcpy(bufptr, cmd->data, (unsigned long)cmd->csize);
        return cmd->csize;
    }
    return 0;
#endif
}

int devSendPacket(int devId, unsigned char *buf, int size)
{
    int ret;
    int addr = devAdresses[devId];

    //printf("i2c_wBuf %i: %i\r\n", devId, size);
    ret = i2c_writeBufferRaw(0, addr, buf, size);
    return ret;
}


void _i2c_read(int devId, unsigned char *buffer, int reqBytes)
{
    int ret;
    ret = i2c_readRaw(0, devId, buffer, reqBytes);

}
