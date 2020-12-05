#include "i2cpacket.h"
#include <onion-i2c.h>
#include <stdio.h>
#include <sys/socket.h>
#include "globals.h"
#include "sock.h"
#include "cmdpacket.h"
#include "gpio18.h"

#include "ctime.h"
#include "camlist.h"

static int devAdresses[C_KT_COUNT] = {0x32, 0x34, 0x36, 0x38};
static unsigned char ibuf[BUF_SZ+1]; // i2c to eth packet buffer

static tCmdBuf i2cbuf[C_KT_COUNT];

static int psz=0;

static cAmList amlist[C_KT_COUNT];

void i2c_init(void)
{
    int i=0;
    for (i=0; i<C_KT_COUNT; i++) {
        bufclear(&i2cbuf[i]);
    }
}

void i2c_poll(void)
{
    int i=0;
    int addr;
    t_id amId;
    //int ret;
   // bool isHere;
    if (!gpio_poll()) return;
    for (i=0; i<C_KT_COUNT; i++) {
        if (!gpio_poll()) break;
        addr = devAdresses[i];
        // poll that i2c has data
        memset(ibuf, 0, C_FULL_PACKET_SZ+1);
        psz = devGetPacket(addr, ibuf);
        if (psz) {
            if (psz==C_MID_PACKET_SZ) { // am on kt
                amId = (ibuf[2] << 8) | ibuf[3];
                if(!amlist[i].add(amId)) {
                    //printf(CLKHD "0x%04x always here\r\n", getms1m(), amId);
                    continue;
                }
                printf(CLKHD "i2c%i 0x%02x: ", getms1m(), i, addr);
                printhex("", ibuf, psz+1);//printPacket(ibuf+1);
            } else {
                printf(CLKHD "i2c%i 0x%02x: ", getms1m(), i, addr);
                printhex("", ibuf, psz+1);//printPacket(ibuf+1);
            }

            sock_send(i, ibuf+1, (size_t)psz);
        }
    } // for i
}

int devGetPacket(int devId, unsigned char *bufptr)
{
    int ret;
    //tCmdBuf *cmd;
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
}

int devSendPacket(int devId, unsigned char *buf, int size)
{
    int ret;
    int addr = devAdresses[devId];

    //printf("i2c_wBuf %i:0x%02x (%i)\r\n", devId, addr, size);
    ret = i2c_writeBufferRaw(0, addr, buf, size);
    return ret;
}

void _i2c_read(int devId, unsigned char *buffer, int reqBytes)
{
    int ret;
    ret = i2c_readRaw(0, devId, buffer, reqBytes);

}
