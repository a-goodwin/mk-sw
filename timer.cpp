#include "timer.h"

timer::timer(int id)
{
    setId(id);
    timeout = nullptr;
}

void timer::setInterval(unsigned long ms)
{
    m_interval = ms;
}

void timer::start()
{
    m_cur = getms();
    m_fin = m_cur+m_interval;
    m_ready = false;
    m_started = true;
}

void timer::start(unsigned long ms)
{
    setInterval(ms);
    start();
}

void timer::stop()
{
    m_started = false;
}

void timer::setTimeoutCallback(void(*fptr)(timer* from))
{
    timeout = fptr;
}

void timer::process()
{
    if (!m_started) return;

    m_cur = getms();
    if (m_cur<m_fin) return;
    m_ready = true;
    if (timeout != nullptr) timeout(this);
}

bool timer::isReady()
{
    return m_ready;
}

bool timer::isStarted()
{
    return m_started;
}

int timer::id() const
{
    return m_id;
}

void timer::setId(int value)
{
    m_id = value;
}
