#include <string.h>

#include "TimerBase.h"

//------------------ CTimerBase --------------
CTimerBase::CTimerBase ()
{   
    m_cCycleFlag = 1;
    m_iIntervalTime = 0;
    m_iExpire = 0;

    m_cNewFlag = 0;

    m_iCmdID = 0;
}

CTimerBase::~CTimerBase ()
{}

void CTimerBase::Reclaim ()
{}

void CTimerBase::SetCycleFlag (char cFlag)
{
    m_cCycleFlag = cFlag;
}

char CTimerBase::GetCycleFlag ()
{
    return m_cCycleFlag;
}

void CTimerBase::SetIntervalTime (unsigned int iIntervalTime)
{
    m_iIntervalTime = iIntervalTime;
}

unsigned int CTimerBase::GetIntervalTime ()
{
    return m_iIntervalTime;
}

CTimerBase & CTimerBase::operator= (CTimerBase &oRth)
{
    if (this != &oRth)
    {
        this->m_cCycleFlag = oRth.m_cCycleFlag;
        this->m_iIntervalTime = oRth.m_iIntervalTime;
    }

    return *this;
}

int CTimerBase::Handle ()
{

    return 0;
}

int CTimerBase::PreHandle()
{
    return 0;
}

int CTimerBase::PosHandle()
{
    return 0;
}

void CTimerBase::SetNewFlag (char cFlag)
{
    m_cNewFlag = cFlag;
}

char CTimerBase::GetNewFlag ()
{
    return m_cNewFlag;
}

void CTimerBase::SetProcessTime (struct timeval *pstTime)
{
    memcpy (&m_stProcessTime, pstTime, sizeof(struct timeval));
}

struct timeval *CTimerBase::GetProcessTime()
{
    return &m_stProcessTime;
}

void CTimerBase::SetCmdID(int iCmdID)
{
    m_iCmdID = iCmdID;
}


