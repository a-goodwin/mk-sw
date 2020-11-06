#ifndef CMDPACKET_H
#define CMDPACKET_H

#define BUF_SZ 256U

#define C_STD_PACKET_SZ 6
#define C_MID_PACKET_SZ 4
#define C_SMALL_PACKET_SZ 2

#define MASTER_SIGNATURE (0xf5U)
#define SLAVE_SIGNATURE (0x5fU)
#define REAR_CAR_SIGNATURE (0xf6U)
#define FRONT_CAR_SIGNATURE (0xf7U)
#define C_HCRC_INIT (0xffU)
#define C_DCRC_INIT (0x00U)

typedef struct {
    int pos; // buffer current position and receive fsm
    int csize; // command size, if applicable
    unsigned short caddr; // command address, if aplicable
    unsigned char cid; // command id, if aplicable
    unsigned char sign; // command signature
    unsigned char hcrc;
    unsigned char dcrc;
    unsigned char data[BUF_SZ];
} tCmdBuf;



extern tCmdBuf latch;//unsigned char i2cbuf[BUF_SZ]; // i2c receive FSM buffer

tCmdBuf* getCmd(void);
void bufclear(tCmdBuf *buf);
void setLatch(tCmdBuf *src);
int processPacketData(tCmdBuf* buf, unsigned char* inbuf, int size);
#endif // CMDPACKET_H