#ifndef FORMS_RTC_H
#define FORMS_RTC_H
#include "messages.h"

namespace Forms 
{

/**
 * Produces MESSAGE_RTC_TICK messages once a second.
 */
class RtcMessageSource : public MessageSource
{
    public:
        Message GetMessage();
    private:
        RtcMessageSource();
        static RtcMessageSource instance;
        static unsigned char last_time;
};


} /* namespace Forms */

#endif /* FORMS_RTC_H */
