#include "cktcontainer.h"

cKtContainer::cKtContainer()
{
    unsigned char i=0;
    for(i=0;i<C_KT_COUNT; i++)
        kts[i] = new cKt(i);
}

cKtContainer::~cKtContainer()
{
    unsigned char i=0;
    for(i=0;i<C_KT_COUNT; i++)
        delete kts[i];

}

void cKtContainer::sendCmd(unsigned char ktIdx, unsigned char *cmd, int size)
{
    kts[ktIdx]->sendCmd(cmd, size);
}

void cKtContainer::sendCmdRep(unsigned char ktIdx, unsigned char *cmd, int size, unsigned int duration, bool waitfor)
{
    kts[ktIdx]->sendCmdRep(cmd, size, duration, waitfor);
}

void cKtContainer::stopCmd(unsigned char ktIdx)
{
    kts[ktIdx]->stopCmd();
}

void cKtContainer::eventloop()
{
    t_ctime ct = getms();
    unsigned char i=0;
    for(i=0;i<C_KT_COUNT; i++) kts[i]->eventloop(ct);
}
