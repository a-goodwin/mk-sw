#ifndef CKT_H
#define CKT_H
#include "cmdpacket.h"
#include "ctime.h"
#include "globals.h"
class cKt
{
public:
    cKt(unsigned char idx);
    ~cKt();

    void setId(unsigned char idx);
    void sendCmd(unsigned char *cmd, int size);
    void sendCmdRep(unsigned char* cmd, int size, unsigned int duration, bool waitfor);
    void stopCmd();
    void eventloop(t_ctime ct);

private:
    unsigned char m_idx=0;
    unsigned int durationMs=0;
    unsigned long stopTimeMs=0;
    unsigned long repTimer=0;
    const static unsigned int repPeriodMs=C_CMD_REPEAT_PERIOD_MS;
    bool cmdActive = false;
    bool infinite = false;

    int curSz=0;
    unsigned char curCmd[BUF_SZ];
};

#endif // CKT_H
