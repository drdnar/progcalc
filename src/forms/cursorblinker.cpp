#include "cursorblinker.h"
#include "crystaltimer.h"

using namespace Forms;

bool CursorBlinker::blink = false;
CursorBlinker CursorBlinker::instance;

CursorBlinker::CursorBlinker()
 : MessageSink(SINK_PRIORITY_HIGH)
{
    MessageLoop::RegisterMessageSink(*this);
}


bool CursorBlinker::SendMessage(Message& message)
{
    if (message.Id == MESSAGE_HW_TIMER_1_EXPIRED)
    {
        blink = !blink;
        MessageLoop::EnqueueMessage({ .Id = MESSAGE_BLINK, .ExtendedCode = blink });
    }
    return false;
}


void CursorBlinker::Restart()
{
    blink = true;
    MessageLoop::EnqueueMessage({ .Id = MESSAGE_BLINK, .ExtendedCode = blink });
    CrystalTimer::Reset();
}
