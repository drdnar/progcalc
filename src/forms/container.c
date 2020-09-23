#define WIDGET Container
#include "widget.h"
#include "container.h"

Widget_def* Container_GetNextItem(const Widget_def* Template)
{
    Widget_def* childDef = &template->FirstChild;
    unsigned char i = template->ChildCount;
    while (i --> 0)
        childDef = childDef->vtable->GetNextItem(childDef);
    return childDef;
}


static void Container_dtor(Widget_t* self)
{
    free(this->Container.ChildCount);
}


Widget_t* WIDGET_ctor(const Widget_def* Template, Widget_t* self, Widget_def** next)
{
    unsigned char i;
    Widget_def* childDef;
    Widget_t* child;
    Widget_def* parent = self->Definition;
    this->Container.ChildCount = template->ChildCount;
    this->Container.Children = calloc(template->ChildCount, sizeof(Widget_t*));
    /* Construct children */
    for (i = 0, childDef = &template->FirstChild; i < template->ChildCount; i++)
        (*this->Container.Children)[i] = childDef->vtable->ctor(childDef, parent, &childDef);
    if (next != NULL)
        *next = childDef;
    return (Widget_t*)self;
}


uint8_t Container_MoveTo(Widget_t* self, uint24_t X, uint8_t Y)
{
/*    unsigned char i;
    Widget_t** child;
    GenericWidget_MoveTo(self, X, Y);
    for (i = 0, child = &this->Children[0]; i < this->ChildCount; i++, child++, X += (*child)->Width)
        (*child)->vtable->MoveTo(*child, X, Y);
*/    return 0;
}


int24_t Container_Paint(Widget_t* self)
{
/*    unsigned char i;
    Widget_t** child;
    for (i = 0, child = &this->Children[0]; i < this->ChildCount; i++, child++)
        (*child)->vtable->Paint(*child);
*/    return 0;
}


int24_t Container_Focus(Widget_t* self)
{
/*    unsigned char i;
    Widget_t** child;
    for (i = 0, child = &this->Children[0]; i < this->ChildCount; i++, child++)
        if (!(*child)->vtable->Focus(*child))
        {
            this->Widget.HasFocus = true;
            return 0;
        }
*/    return 1;
}


int24_t Container_Unfocus(Widget_t* self)
{
/*    unsigned char i;
    Widget_t** child;
    for (i = 0, child = &this->Children[0]; i < this->ChildCount; i++, child++)
        (*child)->vtable->Unfocus(*child);
*/    return 0;
}


int24_t Container_SendInput(Widget_t* self, int24_t messageId)
{
/*    unsigned char i;
    int24_t r;
    Widget_t** child;
    for (i = 0, child = &this->Children[0]; i < this->ChildCount; i++, child++)
        if ((*child)->HasFocus)
            if (r = (*child)->vtable->SendInput(*child, messageId))
                return r;
*/    return 0;
}
