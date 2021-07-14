#ifndef CURSOR_BLINKER_H
#define CURSOR_BLINKER_H

#include "widget.h"
#include "messages.h"

namespace Forms
{

/**
 * Produces MESSAGE_BLINK once a second.
 * TODO: This should use a crystal timer instead of the RTC.
 */
class CursorBlinker : public MessageSink
{
    public:
        bool SendMessage(Message& message);
        /**
         * Checks if the blinking cursor is showing.
         */
        static bool IsShowing() { return blink; }
        /**
         * Resets the blinking cursor so it's showing.
         */
        static void Restart();
    private:
        CursorBlinker();
        /**
         * True if the cursor should be shown, false otherwise.
         */
        static bool blink;
        static CursorBlinker instance;
};

} /* namespace Forms */

#endif /* CURSOR_BLINKER_H */
