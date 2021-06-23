#ifndef FORMS_APD_H
#define FORMS_APD_H

#include "messages.h"

extern "C" const unsigned int ApdDimTime;
extern "C" const unsigned int ApdQuitTime;

namespace Forms
{


/**
 * MESSAGE_APD ExtendedCode IDs.
 */
enum APDType
{
    /**
     * APD timer has been reset.
     * This only triggers after APD_DIM has been fired.
     */
    APD_RESET,
    /**
     * Keyboard has been idle for a while, so the screen is dimmed.
     */
    APD_DIM,
    /**
     * Keyboard has been idle for a long while, so turn off the device
     * (or exit the program).
     */
    APD_TURN_OFF
};


/**
 * Manages anti-battery-kill feature.
 */
class APD : public MessageSink
{
    public:
        bool SendMessage(Message& message);
        /**
         * Manually forces the APD timer to reset.
         */
        static void Reset(void);
        /**
         * Returns true if the APD has fired, and therefore you should clean up
         * and quit.
         */
        static bool Expired(void) { return apd_fired; }
    private:
        APD(void);
        static APD instance;
        static unsigned int timer;
        static bool dimmed;
        static bool apd_fired;
};


} /* namespace Forms */

#endif /* FORMS_APD_H */
