/**
 *   @file     timer_wrapper_impl.h
 *   @brief    定时器封装类
 *   @author   kurtlau
 *   @date     2013-04-08
 */

#ifndef __TIMER_WRAPPER_IMPL_H__
#define __TIMER_WRAPPER_IMPL_H__

#include <sys/time.h>
#include <list>
#include "timer_wrapper.h"

#define MILLISEC_PER_SEC 1000 //1秒有1000毫秒
#define MICROSEC_PER_MS  1000 //1毫秒有1000微秒

#define SLOTS_PER_WHEEL  10
#define SLOT_MASK        (SLOTS_PER_WHEEL - 1)
#define WHEEL_NUM        10

class TimerWrapperImpl : public TimerWrapper
{
public:
    TimerWrapperImpl() : m_tick_count(0){};
	
    virtual ~TimerWrapperImpl(){};

    virtual int AddTimer(TimerBase *timer);
	
    virtual int Tick();
	
private:
    int TimevalInterval(struct timeval& tv1, struct timeval& tv2);
	
    int UpdateLastTick(unsigned int ticks);
    
	unsigned int MsToTick(unsigned int ms);
    
	unsigned int GetTicks(struct timeval &tv1, struct timeval &tv2);
    
	int RunTimer(std::list<TimerBase *>& timerList);
    
	int CascadeTimer(std::list<TimerBase *>& timerList);
    
	int InternalTick();
    
	int InternalAddTimer(TimerBase *timer);

    unsigned int           m_tick_count;
	
    struct timeval         m_last_tick;
    
	struct timeval         m_init_time;
    
	std::list<TimerBase *> m_wheels[WHEEL_NUM][SLOTS_PER_WHEEL];
};

#endif
