#include "ctime.h"

#include <time.h>

unsigned int getms(void)
{
    unsigned long clk_per_ms = CLOCKS_PER_SEC/1000;
    //struct timeval te;
    //gettimeofday(&te, 0);
    unsigned long clk = (unsigned long)clock();
    unsigned long ms = clk/clk_per_ms;
    return (unsigned int)ms;
}
