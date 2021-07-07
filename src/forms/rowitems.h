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
        void Layout();
        /* New routines: */
        /**
         * Empty constructor.
         */
        RowItems();
        static Widget_def* GetNextItem(Widget_def* Template);
        /**
         * Forms initialization magic.
         */
        static Widget* form_ctor(Widget_def* Template, Widget* parent, Widget_def** Next);
        HorizontalAlignment GetAlignment();
        virtual Status SetAlignment(HorizontalAlignment alignment);
    protected:
        RowItems(Widget_def* Template, Widget* Parent, Widget_def** next);
        HorizontalAlignment _alignment;
        unsigned char _padding;
};

} /* namespace Forms */

#endif /* FORMS_ROWITEMS_H */
