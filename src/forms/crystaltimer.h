#ifndef CRYSTAL_TIMER_H
#define CRYSTAL_TIMER_H

#include "messages.h"

namespace Forms
{

/**
 * Handles timing without the crystal timers.
 * This should really implement a queue system so Widgets can schedule multiple
 * events in the future.
 * Yes, indeed, that's something I should implement.  Should.
 * 
 * So this API is very PoC.
 */
class CrystalTimer final : public MessageSource
{
    public:
        Message GetMessage();
        static void Reset();
    private:
        static const unsigned int Frequency = 32768;
        CrystalTimer();
        ~CrystalTimer();
        /**
         * Initialization hook.
         */
        static CrystalTimer instance;
};

} /* namespace Forms */

#endif /* CRYSTAL_TIMER_H */
