#include "ckt.h"
#include "i2cpacket.h"
#include "string.h"
#include "globals.h"
#include "stdio.h"

cKt::cKt(unsigned char idx)
{
    setId(idx);
}

cKt::~cKt()
{

}

void cKt::setId(unsigned char idx)
{
    m_idx = idx;
}

void cKt::sendCmd(unsigned char *cmd, int size)
{
    // stop command sending and send just one command
    cmdActive = false;
    durationMs = 0;
    stopTimeMs = 0;
    infinite = false;
    devSendPacket(m_idx, cmd, size);

}
// TODO: write code for waitfor option - wait for car to repeat command
void cKt::sendCmdRep(unsigned char *cmd, int size, unsigned int duration, bool waitfor)
{
    //arm variables
    memcpy(curCmd, cmd, size);
    curSz = size;
    cmdActive = true;
    repTimer = getms();
    if (duration==0)
        infinite = true;
    else {
        infinite = false;
        durationMs = duration;
        stopTimeMs = repTimer+durationMs;
    }
    if (size>BUF_SZ) size = BUF_SZ;
    devSendPacket(m_idx, curCmd, size);
}

void cKt::stopCmd()
{
    cmdActive = false;
    infinite = false;
}

void cKt::eventloop(t_ctime ct)
{
    //t_ctime ct;
    if (cmdActive) {
        ct = getms();
        // check for duration, if finite
        if (!infinite) {
            if (ct>=stopTimeMs) {// end of cmd
                printf(CLKHD " kt %i cmd stop\r\n",getms1m(), m_idx);
                stopCmd();
                return;
            }
        }
        // check for repeat timeout
        if (ct-repTimer>repPeriodMs) {// repeat cmd
            repTimer = ct;
            devSendPacket(m_idx, curCmd, curSz);
        }
    }
}
