#ifndef FORMS_GUI_H
#define FORMS_GUI_H
#include "widget.h"
#include "container.h"
#include "messages.h"

namespace Forms
{

/**
 * A pointer to a Style that will act as the default Style for all Widgets.
 */
extern Style* const GUI_DefaultStyle;


/**
 * This class handles all user interaction for a Forms GUI.
 * It implements the Widget class so that there is an easy way for the active
 * dialog to get the main window bounds.
 */
class GUI : public MessageSink, public Widget
{
    public:
        static GUI& GetInstance() { return instance; }
        Status Paint();
        Status Unfocus() { return Status::Failure; }
        Status Disable() { return Status::Failure; }
        Status Hide() { return Status::Failure; }
        bool SendInput(Message& message);
        /**
         * Returns GUI singleton.
         */
        bool SendMessage(Message& message);
        /**
         * Maximum number of nested modal dialogs supported.
         */
        static const unsigned char MAX_DIALOGS = 16;
        /**
         * Returns a pointer to the current active dialog, which might be a
         * modal or a top-most dialog.
         */
        static Container* GetActiveDialog() { return active_dialog; }
        /**
         * Returns the Style for the active dialog.
         */
        static const Style& GetActiveStyle();
    private:
        GUI();
        ~GUI();
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
        static void flush_dialogs();
        /**
         * Internal routine that starts a dialog.
         */
        static void begin_dialog(Widget_def*);
        /**
         * Internal routine that cleans up a closed dialog.
         */
        static void end_dialog();
};

} /* namespace Forms */

#endif /* FORMS_GUI_H */
