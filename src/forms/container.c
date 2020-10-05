#define WIDGET Container
#include "widget.inc.h"
#include "container.h"
#define this_Children (*this->Container.Children)
#define this_ChildCount (this->Container.ChildCount)


Widget_def* Container_GetNextItem(const Container_def* Template)
{
    Widget_def* childDef = &Template->FirstChild;
    unsigned char i = Template->ChildCount;
    while (i --> 0)
        childDef = childDef->vtable->GetNextItem(childDef);
    return childDef;
}


void Container_dtor(Widget_t* self)
{
    free(*this->Container.Children);
}


Widget_t* WIDGET_ctor(const Widget_def* Template, Widget_t* self, Widget_def** next)
{
    unsigned char i;
    Widget_def* childDef;
    Widget_t* child;
    this->Container.ActiveIndex = 0;
    this->Container.ChildCount = template->ChildCount;
    this->Container.Children = calloc(template->ChildCount, sizeof(Widget_t*));
    /* Construct children */
    for (i = 0, childDef = &template->FirstChild; i < template->ChildCount; i++)
        this_Children[i] = childDef->vtable->ctor(childDef, self, &childDef);
    if (next != NULL)
        *next = childDef;
    return (Widget_t*)self;
}


uint8_t Container_MoveTo(Widget_t* self, uint24_t X, uint8_t Y)
{
    Widget_t* child;
    Container_Iterator_t iterator;
    X = X - self->X;
    Y = Y - self->Y;
    GenericWidget_MoveTo(self, X, Y);
    for (child = Container_InitializeIterator(self, &iterator); child != NULL; child = Container_IteratorNext(&iterator))
        child->vtable->MoveTo(child, child->X + X, child->Y + Y);
    return 0;
}


int24_t Container_Paint(Widget_t* self)
{
    Widget_t* child;
    Container_Iterator_t iterator;
    for (child = Container_InitializeIterator(self, &iterator); child != NULL; child = Container_IteratorNext(&iterator))
        child->vtable->Paint(child);
    return 0;
}


int24_t Container_Focus(Widget_t* self)
{
    Widget_t* child = this_Children[this->Container.ActiveIndex];
    return child->vtable->Focus(child);
}


bool Container_FocusNext(Widget_t* self)
{
    Container_Iterator_t iterator;
    Container_InitializeIterator(self, &iterator);
    iterator.Index = this->Container.ActiveIndex;
    Widget_t* child = Container_IteratorNext(&iterator);
    while (child)
    {
        if (!child->vtable->Focus(child))
            return true;
        child = Container_IteratorNext(&iterator);
    }
    return false;
}


bool Container_FocusPrevious(Widget_t* self)
{
    Container_Iterator_t iterator;
    Container_InitializeIterator(self, &iterator);
    iterator.Index = this->Container.ActiveIndex;
    Widget_t* child = Container_IteratorPrevious(&iterator);
    while (child)
    {
        if (!child->vtable->Focus(child))
            return true;
        child = Container_IteratorPrevious(&iterator);
    }
    return false;
}


int24_t Container_Unfocus(Widget_t* self)
{
    Widget_t* child = this_Children[this->Container.ActiveIndex];
    return child->vtable->Unfocus(child);
}


int24_t Container_SendInput(Widget_t* self, int24_t messageId)
{
    Widget_t* child = this_Children[this->Container.ActiveIndex];
    return child->vtable->SendInput(child, messageId);
}


Widget_t*  Container_InitializeIterator(Widget_t* self, Container_Iterator_t* iterator)
{
    iterator->Container = this;
    iterator->Index = 0;
    iterator->IsExhausted = false;
    return Container_IteratorFirst(self);
}


Widget_t* Container_IteratorCurrent(Container_Iterator_t* iterator)
{
    Container_t_data* container = &iterator->Container->Container;
    if (iterator->IsExhausted)
        return NULL;
    return container->Children[container->ActiveIndex];
}


Widget_t* Container_IteratorFirst(Container_Iterator_t* iterator)
{
    Container_t_data* container = &iterator->Container->Container;
    if (container->ChildCount == 0)
    {
        iterator->IsExhausted = true;
        return NULL;
    }
    iterator->Index = 0;
    return container->Children[0];
}


Widget_t* Container_IteratorLast(Container_Iterator_t* iterator)
{
    uint8_t i;
    Container_t_data* container = &iterator->Container->Container;
    if (container->ChildCount == 0)
    {
        iterator->IsExhausted = true;
        return NULL;
    }
    i = container->ChildCount - 1;
    iterator->Index = i;
    return container->Children[i];
}


Widget_t* Container_IteratorNext(Container_Iterator_t* iterator)
{
    Container_t_data* container = &iterator->Container->Container;
    if (iterator->IsExhausted)
        return NULL;
    if (iterator->Index + 1 >= container->ChildCount || iterator->Index == 255)
    {
        iterator->IsExhausted = true;
        return NULL;
    }
    return container->Children[++iterator->Index];
}


Widget_t* Container_IteratorPrevious(Container_Iterator_t* iterator)
{
    Container_t_data* container = &iterator->Container->Container;
    if (iterator->IsExhausted)
        return NULL;
    if (iterator->Index == 0)
    {
        iterator->IsExhausted = true;
        return NULL;
    }
    return container->Children[--iterator->Index];
}
