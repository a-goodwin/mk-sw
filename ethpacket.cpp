#include "ethpacket.h"
#include <string.h>
#include <stdio.h>
cEthCmdParser::cEthCmdParser()
{
    bufClear();
}

cEthCmdParser::tEthCmdBuf *cEthCmdParser::getLastCmd()
{
    return &latch;
    m_cmdReady = false;
}

bool cEthCmdParser::cmdReady()
{
    return m_cmdReady;
}

void cEthCmdParser::bufClear()
{
//    ethcmd.pos=0;
    ethcmd.flags=0;
    ethcmd.psize=0;
    ethcmd.ethsign=0;
//    oldCmdFlag = false;
}

void cEthCmdParser::setLatch()
{
    memcpy(&latch, &ethcmd, sizeof(latch));
    m_cmdReady = true;
}

int cEthCmdParser::processEthData(unsigned char *inbuf, int size)
{
    int i;
    u_int8_t cb;
    int cmdBytes;
    if (size<1) return 0;
    if (inbuf[0]==ETH_VER1_SIGNATURE) { // packet ver1
        printf("ethv1p\r\n");
        if (size<ETH_HDRV1_SIZE) return 0; // bad size for ver1 packet hdr
        ethcmd.flags=inbuf[1];
        ethcmd.psize=size-ETH_HDRV1_SIZE;
        ethcmd.ethsign=ETH_VER1_SIGNATURE;
        ethcmd.duration=inbuf[4]+inbuf[5]*0x100U+inbuf[6]*0x10000U+inbuf[7]*0x1000000U;
//        ethcmd.pos = 8;
        if (ethcmd.psize>BUF_SZ) return 0; // error in packet size field
        memcpy(ethcmd.cmd, inbuf+ETH_HDRV1_SIZE, ethcmd.psize);
        setLatch();
        return ethcmd.psize;
    } else if (inbuf[0]==MASTER_SIGNATURE) { // packet ver_old
        printf("ethv0p\r\n");
//        ethcmd.pos=0;
        ethcmd.flags=0;
        ethcmd.psize=size;
        ethcmd.ethsign=MASTER_SIGNATURE;
        ethcmd.duration=0;
        memcpy(ethcmd.cmd, inbuf, size);
        setLatch();
        return ethcmd.psize;
    }
    return 0;
}
