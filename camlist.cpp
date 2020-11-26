#include "camlist.h"
#include <time.h>
#include <string.h>
#include "ctime.h"
//#include <stdio.h>
#include "globals.h"

cAmList::cAmList()
{
    memset(amlist, 0, sizeof(t_id)*AM_LIST_LEN);
    memset(timelist, 0, sizeof(timelist));
}

bool cAmList::add(t_id id)
{
    if (contains(id)) {
        //printf(CLKHD "always has 0x%04x\r\n", getms1m(), id);
        return false;
    }
    int pos = getFreePos();
    if (pos<0) return false;
    amlist[pos] = id;
    timelist[pos] = getms();
    //printf(CLKHD "add 0x%04x to %i at %llu\r\n", getms1m(), id, pos, timelist[pos]);
    return true;
}

bool cAmList::contains(t_id id)
{
    t_id cid;
    renewList();
    for (int i=0; i<AM_LIST_LEN; i++) {
        cid = amlist[i];
        if (cid == 0) continue;
        if (cid == id) return true;
    }
    return false;
}

void cAmList::renewList()
{
    t_id cid;
    unsigned long long tt;
    unsigned long long ct;
    for (int i=0; i<AM_LIST_LEN; i++) {
        cid = amlist[i];
        if (cid==0) continue;
        tt = timelist[i];
        ct = getms();
        if (ct-tt>AM_TOUT_MS) { // clear current
            amlist[i] = 0;
        }
    }
}

int cAmList::getFreePos()
{
    for (int i=0; i<AM_LIST_LEN; i++) {
        if (amlist[i]==0) return i;
    }
    return -1;
}
