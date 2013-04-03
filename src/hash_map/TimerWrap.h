/**
 *   @file     TimerWrap.h
 *   @brief    定时器封装类
 *   @author   kurtliu.JS.IEG.Tencent.
 *   @date     2013-02-01
 *   @note
 */

#ifndef __TIMER_WRAP_H__
#define __TIMER_WRAP_H__

#include <list>

#include <cstdio>
#define LOG_ERR printf
#define LOG_DBG printf

#define MILLISEC_PER_SEC 1000 //1秒有1000毫秒
#define MICROSEC_PER_MS  1000 //1毫秒有1000微秒

#define TICK_INTERVAL    200  //每个TICK之间的时间间隔，单位毫秒

#define TIMER_LIST_NR   256  //必须是2的整次幂
#define TIMER_LIST_MASK (TIMER_LIST_NR - 1)

struct timeval;
class  CTimerBase;
typedef std::list<CTimerBase*> TimerList;

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
    unsigned int HashCode(unsigned int key){return key & TIMER_LIST_MASK;};
    int RunTimer();

    unsigned int   m_tickCount;
    struct timeval m_lastTick;
    TimerList      m_timerList[TIMER_LIST_NR];
};
#endif
