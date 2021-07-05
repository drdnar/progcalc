#ifndef FORMS_DIALOGBOX_H
#define FORMS_DIALOGBOX_H
#include "widget.h"
#include "container.h"

namespace Forms
{

extern "C" const Widget_desc DialogBox_desc;

/**
 * A DialogBox template.
 */
struct DialogBox_def
{
    /**
     * Data common to all widget definitions.
     */
    Widget_def Widget;
    uint24_t MinimumWidth;
    uint8_t MinimumHeight;
    /**
     * Child items list.
     */
    Container_def Contents;
};


/**
 * A modal dialog box.
 */
class DialogBox : public Container
{
    public:
        /* Inherited:
         * GetX
         * GetY
         * GetParent
         * Focus
         * Unfocus
         * SendInput
         * Paint
         * MoveTo
         */
        void Layout();
        /* New routines: */
        /**
         * Empty constructor.
         */
        DialogBox();
        /**
         * Forms initialization magic.
         */
        static Widget_def* GetNextItem(Widget_def* Template);
        /**
         * Forms initialization magic.
         */
        static Widget* form_ctor(Widget_def* Template, Widget* parent, Widget_def** Next);
    protected:
        DialogBox(Widget_def* Template, Widget_def** next);
};

} /* namespace Forms */

#endif /* FORMS_DIALOGBOX_H */
