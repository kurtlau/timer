/**
 *   @file     timer_wrapper_impl.cc
 *   @brief    定时器封装类
 *   @author   kurtlau
 *   @date     2013-04-08
 *   @note     基于哈希表的实现
 */

#include "timer_wrapper_impl.h"
#include <time.h>
#include "timer_base.h"
#include "timer_helper.h"

namespace HashMapImpl
{

using TimerHelper::MsToTick;
using TimerHelper::GetTicks;
using TimerHelper::MILLISEC_PER_SEC;
using TimerHelper::MICROSEC_PER_MS;

int TimerWrapperImpl::UpdateLastTick(unsigned int ticks)
{
  if (0 == ticks)
  {
    return 0;
  }

  struct timeval tv;
  tv.tv_sec  = ticks * GetTickInterval() / MILLISEC_PER_SEC;
  tv.tv_usec = (ticks * GetTickInterval() - tv.tv_sec * MILLISEC_PER_SEC) 
                  * MICROSEC_PER_MS;

  //timeradd(&m_lastTick, &tv, &m_lastTick);
  return 0;
}

int TimerWrapperImpl::AddTimer(TimerBase *timer)
{
  unsigned int ticks  = MsToTick(timer->GetIntervalTime(), GetTickInterval());

  if (0 == ticks)
  {
    return -1;
  }

  unsigned int expire = m_tickCount + ticks;
  timer->SetExpireTick(expire);

  m_timerList[HashCode(expire)].push_back(timer);
  return 0;
}

int TimerWrapperImpl::RunTimer()
{
  TimerList& list = m_timerList[HashCode(m_tickCount)];
  TimerList::iterator iter = list.begin();

  if (iter != list.end())
  {
    time_t now = time(NULL);
  }

  while(iter != list.end())
  {
    TimerBase *timer = *iter;
    if (m_tickCount != timer->GetExpireTick())
    {
      iter++;
      continue;
    }

    iter = list.erase(iter);

    timer->PreRun();
    timer->Run();
    timer->PostRun();
    
  }

  return 0;
}

int TimerWrapperImpl::Tick()
{
  struct timeval now;
  gettimeofday(&now, NULL);

  //第一个tick时初始化m_lastTick
  if (0 == m_lastTick.tv_sec && 0 == m_lastTick.tv_usec)
  {
    m_lastTick = now;
  }

  unsigned int ticks = GetTicks(now, m_lastTick, GetTickInterval());

  for (unsigned int i = 0; i < ticks; ++i)
  {
    RunTimer();
    m_tickCount++;
  }

  UpdateLastTick(ticks);
  return 0;
}

}
