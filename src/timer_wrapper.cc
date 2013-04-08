/**
 *   @file     timer_wrapper.cc
 *   @brief    定时器封装类
 *   @author   kurtlau
 *   @date     2013-04-08
 */

int TimerWrapper::Init(unsigned int tick_interval)
{
	m_tick_interval = tick_interval;
	// TODO: register TimerWrapper::Tick to locoal timer
	return 0;
}