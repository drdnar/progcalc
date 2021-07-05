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
     * Text to display.
     */
    const char* Text;
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
        Status Focus() { return Status::Failure; };
        Status Disable() { return Status::Failure; };
        bool SendInput(Message& message);
        Status Paint();
        ~Label();
        /* New routines: */
        /**
         * This whole hack with the friend _ctor is necessary because I don't
         * feel like dealing with mangled C++ names.
         */
        friend Widget* Label_ctor(Widget_def* Template, Widget* parent, Widget_def** next);
    protected:
        /**
         * Pointer to a C-style string that is displayed by this Label.
         */
        const char* text;
};

} /* namespace Forms */

#endif /* FORMS_LABEL_H */
