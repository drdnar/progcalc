#ifndef FORMS_LABEL_H
#define FORMS_LABEL_H
#include "widget.h"

namespace Forms
{

extern "C" const Widget_desc Label_desc;

/**
 * Describes a widget template.
 */
struct Label_def
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
};

class Label : public Widget
{
    public:
        /* Inherited:
         * MoveTo
         * GetX
         * GetY
         * Unfocus
         * GetParent
         */
        Status Focus(void) { return Status::Failure; };
        WidgetMessage SendInput(WidgetMessage message) { return 0; };
        Status Paint(void);
        ~Label() = default;
        /* New routines: */
        friend Widget* Label_ctor(Widget_def* Template, Widget* parent, Widget_def** next);
    protected:
        /**
         * Font ID used for text in this Label.
         */
        uint8_t _font;
        /**
         * Pointer to a C-style string that is displayed by this Label.
         */
        char* _text;
};

} /* namespace Forms */

#endif /* FORMS_LABEL_H */
