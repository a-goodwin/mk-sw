#ifndef CMDPACKET_H
#define CMDPACKET_H

#ifdef __cplusplus
extern "C" {
#endif


#define BUF_SZ 256U

#define C_FULL_PACKET_SZ 7U
#define C_STD_PACKET_SZ 6U
#define C_MID_PACKET_SZ 4U
#define C_SMALL_PACKET_SZ 2U

#define MASTER_SIGNATURE (0xf5U)
#define SLAVE_SIGNATURE (0x5fU)
#define REAR_CAR_SIGNATURE (0xf6U)
#define FRONT_CAR_SIGNATURE (0x54U)
#define C_HCRC_INIT (0xffU)
#define C_MID_HCRC_INIT (0x33U)
#define C_DCRC_INIT (0x00U)
typedef unsigned short t_id;
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
#pragma pack(push, 1)
typedef struct {
    unsigned char signature;
    unsigned short address;
    unsigned char cmd;
    unsigned char size;
    unsigned char hcrc;
    unsigned char data[BUF_SZ];
} tCmd;
#pragma pack(pop)

extern tCmdBuf latch;//unsigned char i2cbuf[BUF_SZ]; // i2c receive FSM buffer

tCmdBuf* getCmd(void);
void bufclear(tCmdBuf *buf);
void setLatch(tCmdBuf *src);
int processPacketData(tCmdBuf* buf, unsigned char* inbuf, int size);
void printhex(char* str, unsigned char* buf, int sz);
void printPacket(unsigned char* buf);


#ifdef __cplusplus
}
#endif

#endif // CMDPACKET_H
