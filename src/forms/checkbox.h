#ifndef FORMS_CHECKBOX_H
#define FORMS_CHECKBOX_H
#include "widget.h"

namespace Forms
{

extern "C" const Widget_desc Checkbox_desc;

/**
 * Describes a widget template.
 */
struct Checkbox_def
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
     * Pointer to variable this checkbox edits.
     */
    bool* Variable;
};


/**
 * Lets the user toggle a boolean value.
 */
class Checkbox : public Widget
{
    public:
        /* Inherited:
         * MoveTo
         * GetX
         * GetY
         * GetParent
         */
//        Status Focus();
//        Status Unfocus();
        bool SendInput(Message& message);
        Status Paint();
        /* New routines: */
        static Widget* forms_ctor(Widget_def* Template, Widget* parent, Widget_def** next);
        static Widget_def* GetNextItem(Widget_def* Template);
    protected:
        Checkbox(Widget_def* Template, Widget* parent, Widget_def** next);
        /**
         * Pointer to a C-style string that is displayed by this Checkbox.
         */
        const char* text;
        /**
         * Pointer to a simple variable that this Checkbox toggles.
         */
        bool* variable;
};

} /* namespace Forms */

#endif /* FORMS_CHECKBOX_H */
