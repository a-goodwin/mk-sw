
#include "cmdpacket.h"
#include <stdio.h>
#include <string.h>

tCmdBuf latch;//unsigned char i2cbuf[BUF_SZ]; // i2c receive FSM buffer

inline void bufclear(tCmdBuf *buf)
{
    buf->pos=0;
    buf->csize=0;
    buf->cid=0;
    buf->caddr=0;
    buf->hcrc = 0xff;
    buf->dcrc = 0;
    buf->sign = 0;
}

inline tCmdBuf* getCmd(void) {return &latch;}

inline void setLatch(tCmdBuf *src)
{
    memcpy(&latch, src, sizeof(latch));
}

inline int processPacketData(tCmdBuf* buf, unsigned char* inbuf, int size)
{
    // buf - pointer to instance of command receive buffer
    // size - current input stream size if applicable
    // inbuf - current stream buffer with input data
    int i=0;
    unsigned char *pcb;
    printf("fsm %i data\r\n", size);
    for(i=0; i<size; i++) {
        pcb = inbuf+i;
        printf("byte: 0x%02x\r\n", *pcb);
        buf->data[buf->pos] = *pcb;
        switch (buf->pos) {

        case 0: // signature
            if (*pcb!=SLAVE_SIGNATURE && *pcb != FRONT_CAR_SIGNATURE && *pcb != REAR_CAR_SIGNATURE) {
                printf("badsign: 0x%02x\r\n", *pcb);
                // do not needed because already //bufclear(buf);
                break;
            }
            buf->sign = *pcb;
            if (*pcb==SLAVE_SIGNATURE) buf->csize = C_STD_PACKET_SZ;
            else if (*pcb==FRONT_CAR_SIGNATURE) buf->csize = C_MID_PACKET_SZ;
            else if (*pcb==REAR_CAR_SIGNATURE) buf->csize = C_SMALL_PACKET_SZ;
            buf->hcrc = C_HCRC_INIT;
            buf->pos += 1;
            break;

        case 1: // addr hi or 2-nd signature
            if (buf->csize == C_SMALL_PACKET_SZ) { // small packet arrived ok
                if (buf->data[1] != buf->data[0]) { // error if signature
                    printf("No 2-nd F6 in short\r\n");
                    bufclear(buf);
                    break;
                }
                printf("short cmd\r\n");
                setLatch(buf);
                bufclear(buf);
                return latch.csize;
            }
            buf->hcrc ^= *pcb;
            buf->pos += 1;
            break;

        case 2: // addr lo
            buf->caddr = buf->data[1] << 8 | buf->data[2];
            buf->hcrc ^= *pcb;
            buf->pos += 1;
            break;

        case 3: // cmd id or hcrc if mid_command
            if (buf->csize == C_MID_PACKET_SZ) {
                if (buf->hcrc!=*pcb) {
                    bufclear(buf);
                    break;
                }
                printf("mid cmd @0x%04X\r\n", latch.caddr);
                buf->cid = 0x80; // pack
                setLatch(buf);
                bufclear(buf);
                return latch.csize;
            }
            buf->hcrc ^= *pcb;
            buf->pos += 1;
            buf->cid = buf->data[3];
            break;

        case 4: // size
            buf->csize = C_STD_PACKET_SZ+(*pcb)+1; // estimated size, read
            buf->pos += 1;
            buf->hcrc ^= *pcb;
            break;

        case 5: // hcrc
            if (*pcb!=buf->hcrc) { // error in hcrc
                printf("bad dcrc dif 0x%02x\r\n", *pcb ^ buf->hcrc);
                bufclear(buf);
                break;
            }
            if (buf->csize==C_STD_PACKET_SZ) { // zadel for next version oof protocol
                setLatch(buf);
                bufclear(buf);
                printf("six-byte command received\r\n");
                return latch.csize;
            }
            buf->pos += 1;
            buf->dcrc=C_DCRC_INIT;
            break;

        default: // data+dcrc
            buf->dcrc ^= *pcb;
            buf->pos += 1;
            if (buf->pos==buf->csize) {
                if (buf->dcrc!=0) { //bad dcrc
                    printf("bad dcrc dif 0x%02x\r\n", buf->dcrc);
                    bufclear(buf);
                    break;
                }
                printf("big cmd %i bytes cmd 0x%02X addr 0x%04X recved\r\n", buf->csize, buf->cid, buf->caddr);
                setLatch(buf);
                bufclear(buf);
                return latch.csize;
            }
            break;
        } // switch
    } // for
    return 0;
}
