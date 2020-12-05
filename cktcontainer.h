#pragma once
#include "cmdpacket.h"
#include "i2cpacket.h"
#include "globals.h"
#include "ckt.h"

// only for send purposes for a while
// waitfor control not implemented!
class cKtContainer
{
public:
    cKtContainer();
    ~cKtContainer();
    void sendCmd(unsigned char ktIdx, unsigned char* cmd, int size);
    void sendCmdRep(unsigned char ktIdx, unsigned char* cmd, int size, unsigned int duration, bool waitfor);
    void stopCmd(unsigned char ktIdx);
    void eventloop();

private:
    cKt* kts[C_KT_COUNT];

};
