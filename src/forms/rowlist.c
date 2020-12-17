#define WIDGET RowList
#define TYPEID ROW_LIST
#include "widget.inc.h"
#include "rowlist.h"

const WIDGET_vtable_t WIDGET_vtable =
{
    /* Widget */
    {
        sizeof(WIDGET_vtable_t),
        false,
        &GetNextItem,
        &WIDGET_ctor,
        &dtor,
        &MoveTo,
        &Container_Paint,
        &Container_Focus,
        &Container_Unfocus,
        &Container_SendInput
    }
};


static const Widget_def* GetNextItem(const Widget_def* Template)
{
    return Container_GetNextItem(&template->Children);
}


static void dtor(Widget_t* self)
{
    Container_dtor(self);
    GenericWidget_dtor(self);
}


Widget_t* WIDGET_ctor(const Widget_def* Template, Widget_t* parent, Widget_def** next)
{
    WIDGET_t* widget = (WIDGET_t*)malloc(sizeof(WIDGET_t));
    widget->Widget.TypeId = TYPEID;
    widget->Widget.vtable = (Widget_vtable*)&WIDGET_vtable;
    widget->Widget.Definition = Template;
    widget->Widget.Parent = parent;
    widget->Widget.Width = 0;
    widget->Widget.Height = 0;
    /* Construct children */
    Container_ctor(&template->Children, (Widget_t*)widget, next);
    /* Get height and width */
    Container_Iterator_t i;
    for (Widget_t* child = Container_InitializeIterator(widget, &i); !i.IsExhausted; child = Container_IteratorNext(&i))
    {
        if (widget->Widget.Width < child->Width)
            widget->Widget.Width = child->Width;
        widget->Widget.Height += child->Height;
    }
    /* Updating next is taken care of by Container_ctor(). */
    return (Widget_t*)widget;
}


static uint8_t MoveTo(Widget_t* self, uint24_t X, uint8_t Y)
{
    Container_Iterator_t i;
    GenericWidget_MoveTo(self, X, Y);
    for (Widget_t* child = Container_InitializeIterator(self, &i); !i.IsExhausted; child = Container_IteratorNext(&i), Y += child->Height)
        child->vtable->MoveTo(child, X, Y);
    return 0;
}
