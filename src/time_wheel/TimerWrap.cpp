/**
 *   @file     TimerWrap.cpp
 *   @brief    定时器封装类
 *   @author   kurtliu.JS.IEG.Tencent.
 *   @date     2013-02-01
 *   @note
 */

#include "TimerWrap.h"
#include <time.h>
#include <sys/time.h>

int g_iLastCmdID = 0;

int TimerWrap::TimevalInterval(struct timeval& tv1, struct timeval& tv2)
{
    return (tv1.tv_sec - tv2.tv_sec) * MILLISEC_PER_SEC 
                    + (tv1.tv_usec - tv2.tv_usec) / MICROSEC_PER_MS;
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

int TimerWrap::InternalAddTimer(CTimerBase *timer)
{
    unsigned int expire = timer->GetExpireTick();
    unsigned int ticks  = expire - m_tickCount;

    for (int i = 0; i < WHEEL_NUM; ++i)
    {
        if (SLOTS_PER_WHEEL > ticks)
        {
            m_wheels[i][expire % SLOTS_PER_WHEEL].push_back(timer);
            return 0;
        }

        ticks  /= SLOTS_PER_WHEEL;
        expire /= SLOTS_PER_WHEEL;
    }

    LOG_ERR("ticks = %u, out of range\n", ticks);
    return 1;
}

int TimerWrap::AddTimer(CTimerBase *timer)
{
    unsigned int ticks = MsToTick(timer->GetIntervalTime());

    timer->SetExpireTick(ticks + m_tickCount);
    InternalAddTimer(timer);

    return 0;
}

int TimerWrap::CascadeTimer(std::list<CTimerBase*>& timerList)
{
    std::list<CTimerBase*>::iterator iter = timerList.begin();
    while (iter != timerList.end())
    {
        CTimerBase* timer = *iter;

        iter = timerList.erase(iter);
        InternalAddTimer(timer);
    }

    return 0;
}

int TimerWrap::RunTimer(std::list<CTimerBase*>& timerList)
{
    std::list<CTimerBase*>::iterator iter = timerList.begin();

    if (iter != timerList.end())
    {
        time_t now = time(NULL);
        LOG_DBG("=====TICK %u===== at %s", m_tickCount, ctime(&now));
    }

    while (iter != timerList.end())
    {
        CTimerBase* timer = *iter;
        iter = timerList.erase(iter);

        timer->PreHandle();
        timer->Handle();
        g_iLastCmdID = timer->GetCmdID();
        timer->PosHandle();

        if (1 == timer->GetCycleFlag())
        {
            unsigned int ticks = MsToTick(timer->GetIntervalTime());
            timer->SetExpireTick(ticks + m_tickCount);
            InternalAddTimer(timer);
        }
        else
        {
            if (1 == timer->GetNewFlag())
            {
                delete timer;
            }
        }
    }
    return 0;
}

int TimerWrap::InternalTick()
{
    unsigned int slot  = m_tickCount;
    unsigned int index = 0;



    while (0 ==  slot % SLOTS_PER_WHEEL)
    {
        slot /= SLOTS_PER_WHEEL;
        index++;
        CascadeTimer(m_wheels[index][slot % SLOTS_PER_WHEEL]);
    }

    if (0 != index)
    {
        CascadeTimer(m_wheels[index][slot % SLOTS_PER_WHEEL]);
    }

    return RunTimer(m_wheels[0][m_tickCount % SLOTS_PER_WHEEL]);
}

int TimerWrap::Tick()
{
    struct timeval now;
    gettimeofday(&now, NULL);

    //第一个tick时初始化m_lastTick
    if (0 == m_lastTick.tv_sec && 0 == m_lastTick.tv_usec)
    {
        m_lastTick = now;
        return 0;
    }

    unsigned int ticks = GetTicks(now, m_lastTick);

    for (unsigned int i = 0; i < ticks; ++i)
    {
        m_tickCount++;
        InternalTick();
    }

    UpdateLastTick(ticks);
    return 0;
}
