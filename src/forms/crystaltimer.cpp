#include <tice.h>
#include "crystaltimer.h"

using namespace Forms;

CrystalTimer CrystalTimer::instance;

CrystalTimer::CrystalTimer()
{
    MessageLoop::RegisterAsynchronousMessageSource(*this);
    Reset();
}


Message CrystalTimer::GetMessage()
{
    if (timer_ChkInterrupt(1, TIMER_RELOADED))
    {
        timer_AckInterrupt(1, TIMER_RELOADED);
        Reset();
        return { .Id = MESSAGE_HW_TIMER_1_EXPIRED, .ExtendedCode = MESSAGE_NONE };
    }
    return { .Id = MESSAGE_NONE, .ExtendedCode = MESSAGE_NONE };
}


void CrystalTimer::Reset()
{
    // Prevent race condition issues.
    timer_Disable(1);
    timer_Set(1, Frequency / 2);
    timer_SetReload(1, Frequency / 2);
    timer_Enable(1, TIMER_32K, TIMER_0INT, TIMER_DOWN);
}


CrystalTimer::~CrystalTimer()
{
    timer_Disable(1);
}
