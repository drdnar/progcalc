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
        void Layout(void);
        /* New routines: */
        static Widget_def* GetNextItem(Container_def* Template);
        friend Widget* RowList_ctor(Widget_def* Template, Widget* parent, Widget_def** Next);
};

} /* namespace Forms */

#endif /* FORMS_ROWLIST_H */
