#define WIDGET RowItems
#define TYPEID ROW_ITEMS
#include "widget.inc.h"
#include "rowitems.h"

static uint8_t SetWidth(Widget_t* self, uint24_t width);


static const WIDGET_vtable vtable =
{
    /* Widget */
    {
        sizeof(WIDGET_vtable),
        false,
        &GetNextItem,
        &WIDGET_ctor,
        &GenericWidget_dtor,
        &MoveTo,
        &Paint,
        &Focus,
        &Unfocus,
        &SendInput
    },
    &SetWidth
};


static Widget_def* GetNextItem(const Widget_def* Template)
{
    Widget_def* childDef = &template->FirstChild;
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
    WIDGET_t* widget = (WIDGET_t*)malloc(sizeof(WIDGET_t) - sizeof(ptrdiff_t) + sizeof(ptrdiff_t) * template->ChildCount);
    widget->Widget.TypeId = TYPEID;
    widget->Widget.vtable = (Widget_vtable*)&vtable;
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
        if (widget->Widget.Height < child->Height)
            widget->Widget.Height = child->Height;
        widget->Widget.Width += child->Height;
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
    /* Another way to handle this would be to change the vtable. */
    if (definition->Alignment == ROW_ITEMS_LEFT)
    {
        for (i = 0, child = &this->Children[0]; i < this->ChildCount; i++, child++, X += (*child)->Width)
            (*child)->vtable->MoveTo(*child, X, Y);
    }
    else if (definition->Alignment == ROW_ITEMS_LEFT)
    {
        X += this->Widget.Width;
        for (i = 0, child = &this->Children[this->ChildCount - 1]; i < this->ChildCount; i++, child--, X -= (*child)->Width)
            (*child)->vtable->MoveTo(*child, X, Y);
    }
    /* Otherwise, . . . ? */
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


static int24_t Focus(Widget_t* self)
{
    unsigned char i;
    Widget_t** child;
    for (i = 0, child = &this->Children[0]; i < this->ChildCount; i++, child++)
        if (!(*child)->vtable->Focus(*child))
        {
            this->Widget.HasFocus = true;
            return 0;
        }
    return 1;
}


static int24_t Unfocus(Widget_t* self)
{
    unsigned char i;
    Widget_t** child;
    for (i = 0, child = &this->Children[0]; i < this->ChildCount; i++, child++)
        (*child)->vtable->Unfocus(*child);
    return 0;
}


static int24_t SendInput(Widget_t* self, int24_t messageId)
{
    unsigned char i;
    int24_t r;
    Widget_t** child;
    for (i = 0, child = &this->Children[0]; i < this->ChildCount; i++, child++)
        if ((*child)->HasFocus)
            if (r = (*child)->vtable->SendInput(*child, messageId))
                return r;
    return 0;
}


static uint8_t SetWidth(Widget_t* self, uint24_t width)
{
    this->Widget.Width = width;
    if (definition->Alignment == ROW_ITEMS_RIGHT)
        MoveTo(self, self->X, self->Y);
    return 0;
}
