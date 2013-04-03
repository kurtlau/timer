/**
 *   @file     TimerWrap.cpp
 *   @brief    定时器封装类
 *   @author   kurtliu.JS.IEG.Tencent.
 *   @date     2013-02-01
 *   @note
 */

#include "TimerBase.h"
#include "TimerWrap.h"
#include <sys/time.h>
#include <time.h>

int g_iLastCmdID = 0;

int TimerWrap::TimevalInterval(struct timeval& tv1, struct timeval& tv2)
{
    return (tv1.tv_sec - tv2.tv_sec) * MILLISEC_PER_SEC 
                    + (tv1.tv_usec - tv2.tv_usec) / MICROSEC_PER_MS;
}

unsigned int TimerWrap::MsToTick(unsigned int ms)
{
    return (ms - 1)  / TICK_INTERVAL + 1;
}

unsigned int TimerWrap::GetTicks(struct timeval& tv1, struct timeval& tv2)
{
    int ms = TimevalInterval(tv1, tv2);

    if (ms <= 0)
    {
        return 0;
    }

    return MsToTick(ms);
}

int TimerWrap::UpdateLastTick(unsigned int ticks)
{
    if (0 == ticks)
    {
        return 0;
    }

    struct timeval tv;
    tv.tv_sec  = ticks * TICK_INTERVAL / MILLISEC_PER_SEC;
    tv.tv_usec = (ticks * TICK_INTERVAL - tv.tv_sec * MILLISEC_PER_SEC) 
                            * MICROSEC_PER_MS;

    timeradd(&m_lastTick, &tv, &m_lastTick);
    return 0;
}

int TimerWrap::AddTimer(CTimerBase *timer)
{
    unsigned int ticks  = MsToTick(timer->GetIntervalTime());

    if (0 == ticks)
    {
        return -1;
    }

    unsigned int expire = m_tickCount + ticks;
    timer->SetExpireTick(expire);

    m_timerList[HashCode(expire)].push_back(timer);
    return 0;
}

int TimerWrap::RunTimer()
{
    TimerList& list = m_timerList[HashCode(m_tickCount)];
    TimerList::iterator iter = list.begin();

    if (iter != list.end())
    {
        time_t now = time(NULL);
        LOG_DBG("==========tick %u, at %s", m_tickCount, ctime(&now));        
    }

    while(iter != list.end())
    {
        CTimerBase *timer = *iter;
        if (m_tickCount != timer->GetExpireTick())
        {
            iter++;
            continue;
        }

        iter = list.erase(iter);

        timer->PreHandle();
        g_iLastCmdID = timer->GetCmdID();
        timer->Handle();
        timer->PosHandle();

        if (1 == timer->GetCycleFlag())
        {
            AddTimer(timer);
            continue;
        }

        //不需要循环执行
        if (1 == timer->GetNewFlag())
        {
            delete timer;
        }
    }

    return 0;
}

int TimerWrap::Tick()
{
    struct timeval now;
    gettimeofday(&now, NULL);

    //第一个tick时初始化m_lastTick
    if (0 == m_lastTick.tv_sec && 0 == m_lastTick.tv_usec)
    {
        m_lastTick = now;
    }

    unsigned int ticks = GetTicks(now, m_lastTick);

    for (unsigned int i = 0; i < ticks; ++i)
    {
        RunTimer();
        m_tickCount++;
    }

    UpdateLastTick(ticks);
    return 0;
}
