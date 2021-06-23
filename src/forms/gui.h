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
class GUI : public MessageSink
{
    public:
        /**
         * Returns GUI singleton.
         */
        GUI GetInstance(void) { return instance; }
        bool SendMessage(Message& message);
        /**
         * Maximum number of nested modal dialogs supported.
         */
        static const unsigned char MAX_DIALOGS = 16;
        static Container* GetActiveDialog(void) { return active_dialog; }
    private:
        GUI(void);
        ~GUI(void);
        static GUI instance;
        /**
         * Stack of active dialogs.
         * If dialogs[0] is nullptr, then no GUI is active at all!
         */
        static Container* dialogs[MAX_DIALOGS];
        /**
         * Number of active dialogs.
         */
        static unsigned char dialog_count;
        /**
         * Cached pointer to active dialog.
         * This should always be the same as dialogs[dialog_count - 1].
         */
        static Container* active_dialog;
        /**
         * Tears down all active dialogs.
         */
        static void flush_dialogs(void);
        /**
         * Internal routine that starts a dialog.
         */
        static void begin_dialog(Widget_def*);
        /**
         * Internal routine that cleans up a closed dialog.
         */
        static void end_dialog(void);
};

} /* namespace Forms */

#endif /* FORMS_GUI_H */
