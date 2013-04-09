/**
 *   @file     timer_helper.cc
 *   @brief    定时器辅助函数
 *   @author   kurtlau
 *   @date     2013-04-08
 */

#include "timer_helper.h"

namespace TimerHelper
{

int TimevalInterval(const struct timeval& tv1, const struct timeval& tv2)
{
  return (tv1.tv_sec - tv2.tv_sec) * MILLISEC_PER_SEC 
            + (tv1.tv_usec - tv2.tv_usec) / MICROSEC_PER_MS;
}

unsigned int MsToTick(const unsigned int ms, const unsigned int tick_interval)
{
  return (ms - 1)  / tick_interval + 1;
}

unsigned int GetTicks(const struct timeval& tv1, const struct timeval& tv2, const unsigned int tick_interval)
{
  int ms = TimevalInterval(tv1, tv2);

  if (ms <= 0)
  {
      return 0;
  }

  return MsToTick(ms, tick_interval);
}

}