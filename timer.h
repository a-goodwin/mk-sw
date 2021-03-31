#ifndef TIMER_H
#define TIMER_H

#include "ctime.h"

class timer
{
public:
    timer(int id);

    void setInterval(unsigned long ms);
    void start();
    void start(unsigned long ms);
    void stop();

    void setTimeoutCallback(void(*fptr)(timer* from));
    inline void process();
    inline bool isReady();
    inline bool isStarted();
    int id() const;
    void setId(int value);

private:
    int m_id=0;
    ctime m_cur=0;
    ctime m_fin=0;
    ctime m_interval;
    bool m_ready = false;
    bool m_started = false;
    void(* timeout)(timer* from);
};

#endif // TIMER_H
