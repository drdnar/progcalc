#ifndef FORMS_GUI_H
#define FORMS_GUI_H
#include "widget.h"
#include "container.h"
#include "messages.h"

namespace Forms
{

/**
 * This class handles all user interaction for a Forms GUI.
 */
class GUI
{
    public:
        GUI(Widget_def* gui);
        ~GUI(void);
        /**
         * Passes control to the GUI.  The GUI handles all hardware and software
         * events.
         */
        void GiveControl(void);
    private:
        Widget* header = nullptr;
        Container* form = nullptr;
        Widget* footer = nullptr;

};

} /* namespace Forms */

#endif /* FORMS_GUI_H */
