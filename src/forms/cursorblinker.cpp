#include "cursorblinker.h"

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
    if (message.Id == MESSAGE_RTC_TICK)
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
}
