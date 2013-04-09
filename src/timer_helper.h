/**
 *   @file     timer_helper.h
 *   @brief    定时器辅助函数
 *   @author   kurtlau
 *   @date     2013-04-08
 */

#ifndef __TIMER_HELPER_H__
#define __TIMER_HELPER_H__

#include <sys/time.h>

namespace TimerHelper
{

const unsigned int MILLISEC_PER_SEC = 1000; // 1秒有1000毫秒
const unsigned int MICROSEC_PER_MS  = 1000; // 1毫秒有1000微秒

int TimevalInterval(const struct timeval& tv1, const struct timeval& tv2);

unsigned int MsToTick(const unsigned int ms, const unsigned int tick_interval);

unsigned int GetTicks(const struct timeval& tv1, const struct timeval& tv2, const unsigned int tick_interval);
}

#endif