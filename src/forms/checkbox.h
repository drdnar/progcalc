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
     * ID of font to use for this label.
     */
    uint8_t Font;
    /**
     * Text to display.
     */
    char* Text;
    /**
     * Pointer to variable this checkbox edits.
     */
    bool* Variable;
};

class Checkbox : public Widget
{
    public:
        /* Inherited:
         * MoveTo
         * GetX
         * GetY
         * GetParent
         */
        Status Focus();
        Status Unfocus();
        bool SendInput(Message& message);
        Status Paint();
        /* New routines: */
        friend Widget* Checkbox_ctor(Widget_def* Template, Widget* parent, Widget_def** next);
    protected:
        /**
         * Font ID used for text in this Label.
         */
        uint8_t font;
        /**
         * Pointer to a C-style string that is displayed by this Label.
         */
        char* text;
        /**
         * Pointer to a simple variable that this Checkbox toggles.
         */
        bool* variable;
};

} /* namespace Forms */

#endif /* FORMS_CHECKBOX_H */
