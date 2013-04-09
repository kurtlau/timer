/**
 *   @file     timer_wrapper_impl.cc
 *   @brief    定时器封装类
 *   @author   kurtlau
 *   @date     2013-04-08
 */

#include "timer_wrapper_impl.h"
#include <time.h>
#include "timer_base.h"
#include "timer_helper.h"

namespace TimeWheel
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

  //timeradd(&m_last_tick, &tv, &m_last_tick);
  return 0;
}

int TimerWrapperImpl::InternalAddTimer(TimerBase *timer)
{
  unsigned int expire = timer->GetExpireTick();
  unsigned int ticks  = expire - m_tick_count;

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

  return 1;
}

int TimerWrapperImpl::AddTimer(TimerBase *timer)
{
  unsigned int ticks = MsToTick(timer->GetIntervalTime(), GetTickInterval());

  timer->SetExpireTick(ticks + m_tick_count);
  InternalAddTimer(timer);

  return 0;
}

int TimerWrapperImpl::CascadeTimer(std::list<TimerBase *>& timerList)
{
  std::list<TimerBase*>::iterator iter = timerList.begin();
  while (iter != timerList.end())
  {
    TimerBase* timer = *iter;

    iter = timerList.erase(iter);
    InternalAddTimer(timer);
  }

  return 0;
}

int TimerWrapperImpl::RunTimer(std::list<TimerBase *>& timerList)
{
  std::list<TimerBase*>::iterator iter = timerList.begin();

  while (iter != timerList.end())
  {
    TimerBase* timer = *iter;
    iter = timerList.erase(iter);

    timer->PreRun();
    timer->Run();
    timer->PostRun();
  }
  return 0;
}

int TimerWrapperImpl::InternalTick()
{
  unsigned int slot  = m_tick_count;
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

  return RunTimer(m_wheels[0][m_tick_count % SLOTS_PER_WHEEL]);
}

int TimerWrapperImpl::Tick()
{
  struct timeval now;
  gettimeofday(&now, NULL);

  //第一个tick时初始化m_last_tick
  if (0 == m_last_tick.tv_sec && 0 == m_last_tick.tv_usec)
  {
    m_last_tick = now;
    return 0;
  }

  unsigned int ticks = GetTicks(now, m_last_tick, GetTickInterval());

  for (unsigned int i = 0; i < ticks; ++i)
  {
    m_tick_count++;
    InternalTick();
  }

  UpdateLastTick(ticks);
  return 0;
}

}
