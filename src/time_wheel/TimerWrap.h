/**
 *   @file     TimerWrap.h
 *   @brief    定时器封装类
 *   @author   kurtliu.JS.IEG.Tencent.
 *   @date     2013-02-01
 *   @note
 */

#ifndef __TIMER_WRAP_H__
#define __TIMER_WRAP_H__

#include "TimerBase.h"
#include <list>

#include <cstdio>
#define LOG_ERR printf
#define LOG_DBG printf

#define MILLISEC_PER_SEC 1000 //1秒有1000毫秒
#define MICROSEC_PER_MS  1000 //1毫秒有1000微秒

#define TICK_INTERVAL    200  //每个TICK之间的时间间隔，单位毫秒

#define SLOTS_PER_WHEEL  10
#define SLOT_MASK        (SLOTS_PER_WHEEL - 1)
#define WHEEL_NUM        10    //能够支持到200毫秒~2 * 10^9秒的定时时长

struct timeval;

class TimerWrap
{
public:
    TimerWrap() : m_tickCount(0){};
    ~TimerWrap(){};

    int AddTimer(CTimerBase *timer);
    int Tick();

    static TimerWrap* Instance(){static TimerWrap timerWrap; return &timerWrap;};

private:
    int TimevalInterval(struct timeval& tv1, struct timeval& tv2);
    int UpdateLastTick(unsigned int ticks);
    unsigned int MsToTick(unsigned int ms);
    unsigned int GetTicks(struct timeval &tv1, struct timeval &tv2);
    int RunTimer(std::list<CTimerBase*>& timerList);
    int CascadeTimer(std::list<CTimerBase*>& timerList);
    int InternalTick();
    int InternalAddTimer(CTimerBase *timer);

    unsigned int          m_tickCount;
    struct timeval        m_lastTick;
    struct timeval        m_initTime;
    std::list<CTimerBase*> m_wheels[WHEEL_NUM][SLOTS_PER_WHEEL];
};

#endif
