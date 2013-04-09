/**
 *   @file     timer_wrapper_impl.h
 *   @brief    定时器封装类
 *   @author   kurtlau
 *   @date     2013-04-08
 *   @note     基于哈希表的实现
 */

#ifndef __TIMER_WRAP_H__
#define __TIMER_WRAP_H__

#include <sys/time.h>
#include <list>
#include "timer_wrapper.h"

#define TICK_INTERVAL    200  //每个TICK之间的时间间隔，单位毫秒

#define TIMER_LIST_NR   256  //必须是2的整次幂
#define TIMER_LIST_MASK (TIMER_LIST_NR - 1)

class  TimerBase;
typedef std::list<TimerBase *> TimerList;

namespace HashMapImpl
{

class TimerWrapperImpl : public TimerWrapper
{
public:
  TimerWrapperImpl() : m_tickCount(0){};
  
  ~TimerWrapperImpl(){};

  virtual int AddTimer(TimerBase *timer);

  virtual int Tick();

private:
  int UpdateLastTick(unsigned int ticks);
  
  unsigned int HashCode(unsigned int key){return key & TIMER_LIST_MASK;};
  
  int RunTimer();

  unsigned int   m_tickCount;
  struct timeval m_lastTick;
  TimerList      m_timerList[TIMER_LIST_NR];
};

}
#endif
