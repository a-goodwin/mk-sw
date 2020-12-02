#include "ethpacket.h"
#include <string.h>
#include <stdio.h>
cEthCmdParser::cEthCmdParser()
{
    bufClear();
}

cEthCmdParser::tEthCmdBuf *cEthCmdParser::getLastCmd()
{
    m_cmdReady = false;
    return &latch;
}

bool cEthCmdParser::cmdReady()
{
    return m_cmdReady;
}

void cEthCmdParser::bufClear()
{
    ethcmd.pos=0;
    ethcmd.flags=0;
    ethcmd.hsize=0;
    ethcmd.ethsign=0;
    oldCmdFlag = false;
    bufclear(&ethcmd.cmd);
}

void cEthCmdParser::setLatch()
{
    memcpy(&latch, &ethcmd, sizeof(latch));
}

int cEthCmdParser::processEthData(unsigned char *inbuf, int size)
{
    int i;
    u_int8_t cb;
    int cmdBytes;
    if (size<1) return 0;
    if (inbuf[0]==ETH_VER1_SIGNATURE) { // packet ver1
        if (size<ETH_HDRV1_SIZE) // bad size for ver1 packet hdr
        ethcmd.flags=inbuf[1];
        ethcmd.hsize=8;
        ethcmd.ethsign=ETH_VER1_SIGNATURE;
        ethcmd.duration=inbuf[4]+inbuf[5]*0x100U+inbuf[6]*0x10000U+inbuf[7]*0x1000000U;
        ethcmd.pos = 8;
        cmdBytes = processPacketData(&cmdBuf, inbuf+ethcmd.pos, size-ethcmd.pos);
        if (cmdBytes) { // uart part of command received ok

        }
    } else if (inbuf[0]==MASTER_SIGNATURE) { // packet ver_old
        ethcmd.pos=0;
        ethcmd.flags=0;
        ethcmd.hsize=0;
        ethcmd.ethsign=MASTER_SIGNATURE;
        ethcmd.duration=0;
        cmdBytes = processPacketData(&cmdBuf, inbuf, size);
    }

    for (i=0; i<size; i++) {
        cb = inbuf[i];
        switch (ethcmd.pos) {
        case 0: // signature
            if (cb==ETH_VER1_SIGNATURE) {
                printf("ethver1 pkt\r\n");
                ethcmd.ethsign=cb;
                ethcmd.pos++;
                break;
            } else if (cb==MASTER_SIGNATURE) {// old proto
                printf("eth old pkt\r\n");
                ethcmd.flags=0;
                ethcmd.hsize=8;
                ethcmd.ethsign=ETH_VER1_SIGNATURE;
                ethcmd.duration=0;
                ethcmd.pos=8; // step to "default" pos
                oldCmdFlag = true;
                break;
            } else {
                printf("ethpkt bad sign: 0x%02x\r\n", cb);
                bufClear();
                break;
            } // no correct sign
        case 1: // flags
            ethcmd.flags = cb;
            ethcmd.pos++;
            break;
        case 2: // hdrsize
            ethcmd.hsize = cb;
            if (ethcmd.hsize!=8) { // bad size
                printf("ethcmd bad hsize: 0x%02x\r\n", cb);
                bufClear();
                break;
            }
            ethcmd.pos++;
            break;
        case 3: // reserved
            ethcmd.pos++;
            break;
        case 4: // duration 0
            ethcmd.duration = cb;
            ethcmd.pos++;
            break;
        case 5: // duration 1
            ethcmd.duration += ((u_int32_t)cb) << 8U;
            ethcmd.pos++;
            break;
        case 6: // duration 2
            ethcmd.duration += ((u_int32_t)cb) << 16U;
            ethcmd.pos++;
            break;
        case 7: // duration 3
            ethcmd.duration += ((u_int32_t)cb) << 24U;
            ethcmd.pos++;
            break;
        default: // old proto command
            processEthData()
            break;
        } // switch
    } // for i
}
