#include "ctime.h"

#include <sys/time.h>
//#include <stdio.h>
 // TODO: rewrite properly!!!
ctime getms(void)
{
    struct timeval te;
    gettimeofday(&te, nullptr);
    //printf("getms: %lu, %lu\r\n", te.tv_sec, te.tv_usec);

    ctime  ms = te.tv_sec*1000 + te.tv_usec/1000;
    return ms;
}

unsigned long getms1m()
{
    return getms()%1000000000;
}
