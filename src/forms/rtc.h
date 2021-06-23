#ifndef FORMS_RTC_H
#define FORMS_RTC_H
#include "messages.h"

namespace Forms 
{

class RtcMessageSource : public MessageSource
{
    public:
        Message GetMessage(void);
    private:
        RtcMessageSource(void);
        static RtcMessageSource instance;
        static unsigned char last_time;
};


} /* namespace Forms */

#endif /* FORMS_RTC_H */
