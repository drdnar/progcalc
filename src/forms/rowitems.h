#ifndef FORMS_ROWITEMS_H
#define FORMS_ROWITEMS_H
#include "widget.h"
#include "container.h"

namespace Forms
{

extern "C" const Widget_desc RowItems_desc;

/**
 * Describes a widget template.
 */
struct RowItems_def
{
    /**
     * Data common to all widget definitions.
     */
    Widget_def Widget;
    /**
     * Instructs RowItems how to do layout.
     */
    HorizontalAlignment Alignment;
    /**
     * Extra horizontal spacing to add between items.
     */
    uint8_t Padding;
    /**
     * Child items list.
     */
    Container_def Contents;
};

class RowItems : public Container
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
        friend Widget* RowItems_ctor(Widget_def* Template, Widget* parent, Widget_def** Next);
        HorizontalAlignment GetAlignment(void);
        virtual Status SetAlignment(HorizontalAlignment alignment);
    protected:
        HorizontalAlignment _alignment;
        unsigned char _padding;
};

} /* namespace Forms */

#endif /* FORMS_ROWITEMS_H */
