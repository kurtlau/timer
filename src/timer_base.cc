/**
 *   @file     timer_base.cc
 *   @brief    定时器基类
 *   @author   kurtlau
 *   @date     2013-04-08
 */

#include "timer_base.h"

TimerBase & TimerBase::operator= (const TimerBase &timer)
{
    if (this != &timer)
    {
        this->m_expire = timer.m_expire;
        this->m_process_time = timer.m_process_time;
    }

    return *this;
}
