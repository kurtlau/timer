#include "TimerBase.h"
#include "TimerWrap.h"
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <time.h>

class Timer: public CTimerBase
{
public:
    Timer(){m_uiCount = 0;};
    ~Timer(){};

    virtual int Handle();

private:
    unsigned int m_uiCount;
};

int Timer::Handle()
{
    printf("timer %d, interval %u running %u times\n", 
                        m_iCmdID, m_iIntervalTime / 200, ++m_uiCount);
    return 0;
}

int main(int argc, char const *argv[])
{
    srand(time(NULL));
    for (int i = 0; i < 5; ++i)
    {
        Timer *p = new Timer();
        p->SetCmdID(i);
        p->SetCycleFlag(1);
        p->SetNewFlag(1);
        p->SetIntervalTime((rand()%30 + 1) * 200);

        TimerWrap::Instance()->AddTimer(p);
    }

    for (int i = 0; i < 1000; ++i)
    {
        TimerWrap::Instance()->Tick();
        usleep(200000);
    }
    return 0;
}
