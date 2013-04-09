/**
 *   @file     timer_wrapper.h
 *   @brief    定时器封装类
 *   @author   kurtlau
 *   @date     2013-04-08
 */

#ifndef __TIMER_WRAPPER_H__
#define __TIMER_WRAPPER_H__

class TimerBase;

class TimerWrapper
{
public:
  TimerWrapper(){};

	virtual ~TimerWrapper(){};

	int Init(unsigned int tick_interval);

  virtual int AddTimer(TimerBase *timer) = 0;

	virtual int Tick() = 0;
	
	unsigned int GetTickInterval(){return m_tick_interval;};
	
	void SetTickInterval(unsigned int tick_interval){m_tick_interval = tick_interval;};

private:
	unsigned int m_tick_interval;
};
#endif
