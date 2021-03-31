#include "eventprocessor.h"

CEventProcessor::CEventProcessor()
{
    for( int i=0; i<MAX_TIMERS; i++) timers[i]=0;
}

void CEventProcessor::renewFromDb(char *txtFileBuffer)
{

}

void CEventProcessor::eventloop()
{
    // process timers
    for (int i=0; i<MAX_TIMERS; i++) {
        if (timers[i]!=nullptr) {
            timers[i]->process();
            if (!timers[i]->isStarted()) {
                delete timers[i];
                timers[i]=nullptr;
            }
        }
    }
}

void CEventProcessor::slEvent(int evType, int srcId, int dstId, char *data, int dsize)
{

}

void CEventProcessor::eventFilter(int evType, int srcId, int dstId, char *data, int dsize)
{

}

void CEventProcessor::slDoAction(int acId)
{

}

int CEventProcessor::startTimer(ctime intervalMs, void(*fptr)(timer* from))
{
    timer* tmr=nullptr;
    int curTimerId=-1;
    for (int i=0; i<MAX_TIMERS; i++) {
        if (timers[i]==0) {
            curTimerId = i;
            tmr = new timer(curTimerId);
            timers[i] = tmr;
            break;
        }
    }
    tmr->setTimeoutCallback(fptr);
    tmr->start(intervalMs);
    return curTimerId;
}
