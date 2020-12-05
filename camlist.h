#ifndef CAMLIST_H
#define CAMLIST_H

#include "cmdpacket.h"

#define AM_LIST_LEN 8
#define AM_TOUT_MS 2500
class cAmList
{
public:
    cAmList();
    bool add(t_id id);
    bool contains(t_id id);
private:
    void renewList(void);
    int getFreePos();
    //unsigned long curTime(); // returns current system time in ms
private:
    t_id amlist[AM_LIST_LEN];
    unsigned long long timelist[AM_LIST_LEN];
};

#endif // CAMLIST_H
