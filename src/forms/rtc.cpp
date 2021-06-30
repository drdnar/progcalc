#include "rtc.h"
#include <tice.h>

using namespace Forms;

unsigned char RtcMessageSource::last_time;
RtcMessageSource RtcMessageSource::instance;

RtcMessageSource::RtcMessageSource()
{
    MessageLoop::RegisterSynchronousMessageSource(*this);
    last_time = rtc_Seconds;
}


Message RtcMessageSource::GetMessage()
{
    unsigned char seconds = rtc_Seconds;
    unsigned char delta = seconds - last_time;
    if (delta == 0)
        return { .Id = MESSAGE_NONE, .ExtendedCode = MESSAGE_NONE};
    // else
    last_time = seconds;
    if (delta >= 60)
        delta += 60;
    return { .Id = MESSAGE_RTC_TICK, .ExtendedCode = delta };
}
