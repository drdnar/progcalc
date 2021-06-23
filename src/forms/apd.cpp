#include "apd.h"
#include "../ez80.h"

using namespace Forms;

APD APD::instance;

unsigned int APD::timer;

bool APD::dimmed { false };

bool APD::apd_fired { false };

/**
 * Exits the program, cleaning up stuff.
 */
extern "C" void ExitClean(void); /* I don't really have a better idea where to put this declaration. */


APD::APD(void) : MessageSink(SINK_PRIORITY_VERY_HIGH)
{
    MessageLoop::RegisterMessageSink(*this);
    Reset();
}


void APD::Reset(void)
{
    if (dimmed)
    {
        Lcd_Bright();
        dimmed = false;
        MessageLoop::EnqueueMessage({ .Id = MESSAGE_APD, .ExtendedCode = APD_RESET });
    }
    timer = GetRtcSeconds() + ApdDimTime;
}


bool APD::SendMessage(Message& message)
{
    if (message.Id == MESSAGE_RAW_KEY)
        Reset();
    else if (message.Id == MESSAGE_RTC_TICK)
    {
        if (RtcTimer_Expired(timer))
        {
            if (!dimmed)
            {
                dimmed = true;
                Lcd_Dim();
                timer = GetRtcSeconds() + ApdQuitTime;
                MessageLoop::EnqueueMessage({ .Id = MESSAGE_APD, .ExtendedCode = APD_DIM });
            }
            else
            {
                apd_fired = true;
                MessageLoop::EnqueueMessage({ .Id = MESSAGE_APD, .ExtendedCode = APD_TURN_OFF });
            }
        }
    }
    return false;
}
