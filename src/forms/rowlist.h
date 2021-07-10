#ifndef FORMS_ROWLIST_H
#define FORMS_ROWLIST_H
#include "widget.h"
#include "container.h"

namespace Forms
{

extern "C" const Widget_desc RowList_desc;

/**
 * Describes a widget template.
 */
struct RowList_def
{
    /**
     * Data common to all widget definitions.
     */
    Widget_def Widget;
    /**
     * Child items list.
     */
    Container_def Contents;
};

class RowList : public Container
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
        Status SetWidth(x_t width);
        void Layout();
        /* New routines: */
        /**
         * Empty constructor.
         */
        RowList();
        static Widget_def* GetNextItem(Widget_def* Template);
        /**
         * Forms initialization magic.
         */
        static Widget* form_ctor(Widget_def* Template, Widget* parent, Widget_def** Next);
    protected:
        RowList(Widget_def* Template, Widget* Parent, Widget_def** next);
};

} /* namespace Forms */

#endif /* FORMS_ROWLIST_H */
