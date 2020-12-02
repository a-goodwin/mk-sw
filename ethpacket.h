#ifndef ETHPACKET_H
#define ETHPACKET_H
#ifdef __cplusplus
extern "C" {
#endif
#include <sys/types.h>
#include "cmdpacket.h"

#define ETH_VER1_SIGNATURE (0x01U)
#define ETH_HDRV1_SIZE (8U)
#pragma pack(push, 1)
class cEthCmdParser
{
public:
    typedef struct tHdrV1
    {
        u_int8_t sign; // 0
        u_int8_t flags; // 1
        u_int8_t hdrsize; // 2
        u_int8_t reserved; // 3
        u_int32_t duration; // 4-7
        tHdrV1() {sign=0;flags=0;hdrsize=0;reserved=0;duration=0;}
    };
    #pragma pack(pop)

    typedef struct tEthCmdBuf {
        u_int16_t pos; // buffer current position and receive fsm
        u_int8_t hsize; // eth packet header size, if applicable
        u_int8_t flags;
        u_int32_t duration;
        u_int8_t ethsign; // command signature
        tCmdBuf cmd;
    };

private:
    tEthCmdBuf latch;
    tEthCmdBuf ethcmd;
    tCmdBuf cmdBuf;
    bool m_cmdReady = false;
    bool oldCmdFlag = false;
public:
    cEthCmdParser();

    tEthCmdBuf* getLastCmd(void);
    bool cmdReady();

private:
    void bufClear();
    void setLatch();
    int processEthData(unsigned char* inbuf, int size);
};

#ifdef __cplusplus
}
#endif
#endif // ETHPACKET_H
