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
        &GenericWidget_dtor,
        &MoveTo,
        &Paint,
        &GenericWidget_Focus,
        &GenericWidget_Unfocus,
        &SendInput
    }
};


static const Widget_def* GetNextItem(const Widget_def* Template)
{
    const Widget_def* childDef = &template->FirstChild;
    unsigned char i = template->ChildCount;
    while (i --> 0)
        childDef = childDef->vtable->GetNextItem(childDef);
    return childDef;
}


Widget_t* WIDGET_ctor(const Widget_def* Template, Widget_t* parent, Widget_def** next)
{
    unsigned char i;
    Widget_def* childDef;
    Widget_t* child;
    WIDGET_t* widget = (WIDGET_t*)malloc(sizeof(WIDGET_t) - sizeof(Widget_t*) + sizeof(Widget_t*) * template->ChildCount);
    widget->Widget.TypeId = TYPEID;
    widget->Widget.vtable = (Widget_vtable*)&WIDGET_vtable;
    widget->Widget.Definition = Template;
    widget->Widget.Parent = parent;
    widget->Widget.Width = 0;
    widget->Widget.Height = 0;
    widget->ChildCount = template->ChildCount;
    /* Construct children */
    for (i = 0, childDef = &template->FirstChild; i < template->ChildCount; i++)
    {
        child = childDef->vtable->ctor(childDef, (Widget_t*)widget, &childDef);
        widget->Children[i] = child;
        if (widget->Widget.Width < child->Width)
            widget->Widget.Width = child->Width;
        widget->Widget.Height += child->Height;
    }
    if (next != NULL)
        *next = childDef;
    return (Widget_t*)widget;
}


static uint8_t MoveTo(Widget_t* self, uint24_t X, uint8_t Y)
{
    unsigned char i;
    Widget_t** child;
    GenericWidget_MoveTo(self, X, Y);
    for (i = 0, child = &this->Children[0]; i < this->ChildCount; i++, child++, Y += (*child)->Height)
        (*child)->vtable->MoveTo(*child, X, Y);
    return 0;
}


static int24_t Paint(Widget_t* self)
{
    unsigned char i;
    Widget_t** child;
    for (i = 0, child = &this->Children[0]; i < this->ChildCount; i++, child++)
        (*child)->vtable->Paint(*child);
    return 0;
}


static int24_t SendInput(Widget_t* self, int24_t messageId)
{
    if (!this->Widget.HasFocus)
        return 0;
    
    return 0;
}
