
/**
*   @brief  定时器逻辑处理基类。所有定时器建议从这里继承。需要支持只有回调函数，没有
*/
#ifndef _CTIMERBASE_H_
#define _CTIMERBASE_H_

#include <sys/time.h>

class CTimerBase
{
public:
    CTimerBase();
    virtual ~CTimerBase ();

    /**
    *   @brief  
    */
    virtual int Handle ();
    
    virtual void Reclaim ();

    CTimerBase & operator= (CTimerBase &oRth);
    
    void SetCycleFlag (char cFlag);
    char GetCycleFlag ();

    void SetIntervalTime (unsigned int iIntervalTime);
    unsigned int GetIntervalTime ();

    void SetNewFlag (char cFlag);
    char GetNewFlag ();

    void SetProcessTime (struct timeval *stTime);
    struct timeval *GetProcessTime ();

    void SetCmdID(int iCmdID);
    int GetCmdID(){ return m_iCmdID; }

    virtual int PreHandle();
    virtual int PosHandle();

    void SetExpireTick(unsigned int expire){m_iExpire = expire;};
    unsigned int GetExpireTick(){return m_iExpire;};
    
protected:
    char         m_cCycleFlag;   //循环标示。0 不循环，1循环。
    unsigned int m_iIntervalTime;   // 定时时间.单位:毫秒 
    unsigned int m_iExpire;         // 相对于TimerWrap的Init以后的tick数

    struct timeval m_stProcessTime; // 处理的时间。

    char         m_cNewFlag;     // 动态分配标识，1 mgr自己new出来的。

    int          m_iCmdID;
};

#endif


