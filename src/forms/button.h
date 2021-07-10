#ifndef FORMS_BUTTON_H
#define FORMS_BUTTON_H
#include "widget.h"
#include "textmanager.h"

namespace Forms
{

extern "C" const Widget_desc Button_desc;

class Button;

/**
 * A button that a user can press to trigger an action.
 */
struct Button_def
{
    /**
     * Data common to all widget definitions.
     */
    Widget_def Widget;
    /**
     * Text to display.
     */
    const char* Text;
    /**
     * Can be used to force a Button to be bigger for layout purposes.
     */
    uint24_t MinimumWidth;
    /**
     * An arbitrary number that can help identify a particular Button.
     */
    uint8_t ID;
    /**
     * Callback to execute when the Button is pressed.
     * @param sender Pointer to the Button which was pressed.  The handler must
     * figure out what to do based on the Button's data.
     */
    void (*OnPress)(Button& sender);
};


/**
 * A button that a user can press to trigger an action.
 */
class Button : public Widget
{
    public:
        /* Inherited:
         * MoveTo
         * GetX
         * GetY
         * Unfocus
         * GetParent
         */
        bool SendInput(Message& message);
        Status Paint();
        Button();
        ~Button();
        /* New routines: */
        /**
         * Forms initialization magic.
         */
        static Widget_def* GetNextItem(Widget_def* Template);
        /**
         * Forms initialization magic.
         */
        static Widget* forms_ctor(Widget_def* Template, Widget* parent, Widget_def** next);
        /**
         * Executes the Button's action.
         */
        virtual void Press();
        /**
         * Asks the button for its ID, an arbitrary number that helps identify
         * its role in a Form.
         */
        virtual uint8_t GetID() { return ((Button_def*)definition)->ID; }
    protected:
        /**
         * Forms magic.
         */
        Button(Widget_def* Template, Widget* parent, Widget_def** next);
        /**
         * Fetches associated text.
         */
        const char* GetText() { return ((Button_def*)definition)->Text; }
        /**
         * Caches the width of the text for centering.
         */
        unsigned int textWidth;
        /**
         * Font ID used for text in this Button.
         */
        //uint8_t font;
        /**
         * Pointer to a C-style string that is displayed by this Button.
         */
        //const char* text;
        /**
         * The Button's ID provides additional information about the Button's
         * function in a particular Form.
         */
        //uint8_t id;
        /**
         * This callback does the Button's actual work.
         */
        //void (*onPress)(Button& sender);
};

} /* namespace Forms */

#endif /* FORMS_BUTTON_H */
