#ifndef FORMS_EVENT_SOURCE_H
#define FORMS_EVENT_SOURCE_H

#include "widget.h"

namespace Forms
{

/**
 * Interface for a class that provides events to a GUI.
 */
class EventSource
{
    public:
        /**
         * Requests an event source to look for an event.
         */
        virtual WidgetMessage GetEvent(void);
        EventSource(void);
        virtual ~EventSource()/* = default*/;
    //private:
        unsigned char derp;
};

} /* namespace Forms */

#endif /* FORMS_EVENT_SOURCE_H */
