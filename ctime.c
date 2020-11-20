#include "ctime.h"

#include <time.h>

unsigned int getms(void)
{
    //struct timeval te;
    //gettimeofday(&te, 0);
    unsigned int ms = (clock()/(CLOCKS_PER_SEC/1000))%1000000;
    return ms;
}
