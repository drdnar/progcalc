#include "container.h"
#include "style.h"
#include <string.h>

using namespace Forms;

/**
 * Returns a somewhat larger number, using a spurious coinage.
 */
#define EMBIGGEN(x) ( x  ?  ((x) * 3 + 1) / 2  :  2 )

Widget_def* Container::GetNextItem(Container_def* Template)
{
    if (Template == nullptr)
        return nullptr;
    Widget_def* def = &Template->FirstChild;
    for (uint8_t i = Template->ChildCount; i > 0; i--)
        def = def->TypeDescriptor->GetNextWidget(def);
    return def;
}


Container::Container() : Container(4)
{
    // Leave everything at defaults
}


Container::Container(Container_size_t initialSize)
{
    if (initialSize == 0)
        size = 4;
    else
        size = initialSize;
    children = new Widget*[size];
    Widget** child = &children[0];
    for (Container_size_t i = size; i > 0; i--, child++)
        *child = nullptr;
}


Container::Container(Container_def* Template, Widget* Parent, Widget_def** Next)
 : Widget()
{
    parent = Parent;
    Widget_def* next = &Template->FirstChild;
    size = Template->ChildCount;
    Style::constructify(next, getStylePointer(), nullptr);
    count = 0;
    children = new Widget*[size];
    Widget** child = &children[0];
    for (Container_size_t i = size; i > 0; i--)
    {
        if (!(next->TypeDescriptor->Flags & WIDGET_FLAG_NON_INSTANTIABLE))
        {
            *child++ = next->TypeDescriptor->ctor(next, this, &next);
            count++;
        }
        else
            next = next->TypeDescriptor->GetNextWidget(next);
    }
    if (Next != nullptr)
        *Next = next;
    hasFocus = true;
    FocusFirst();
    Unfocus();
}


Container::~Container()
{
    if (!children)
        return;
    Widget** child = children;
    for (Container_size_t i = count; i > 0; i--, child++)
        delete *child;
    delete[] children;
}


Status Container::MoveTo(x_t nx, y_t ny)
{
    /* Signed or unsigned, doesn't really matter here, just let it overflow. */
    x_t dx = nx - x;
    y_t dy = ny - y;
    x = nx;
    y = ny;
    if (!count)
        return Status::Success;
    Widget** item = children;
    for (Container_size_t i = count; i > 0; i--, item++)
        (*item)->MoveTo((*item)->x + dx, (*item)->y + dy);
    return Status::Success; 
}


Status Container::Focus()
{
    if (!count)
        return Status::Failure;
    Status r = children[active_index]->Focus();
    hasFocus = r != Status::Failure;
    return r;
}


Status Container::Unfocus()
{
    if (!count)
        return Status::Success;
    Status r = children[active_index]->Unfocus();
    hasFocus = r == Status::Failure;
    return r;
}


bool Container::SendInput(Message& message)
{
    if (count && hasFocus)
        return children[active_index]->SendInput(message);
    return false;
}


bool Container::Broadcast(Message& message)
{
    if (!count)
        return false;
    Widget** item = children;
    for (Container_size_t i = count; i > 0; i--, item++)
    {
        Container* container = dynamic_cast<Container*>(*item);
        bool r;
        if (container)
            r = container->Broadcast(message);
        else
            r = (*item)->SendInput(message);
        if (r)
            return true;
    }
    return false;
}


void Container::SetDirtyAll()
{
    SetDirty();
    really_dirty = true;
    if (!count)
        return;
    Widget** item = children;
    for (Container_size_t i = count; i > 0; i--, item++)
    {
        Container* container = dynamic_cast<Container*>(*item);
        if (container)
            container->SetDirtyAll();
        else
            (*item)->SetDirty();
    }
}


Status Container::Paint()
{
    if (!dirty || !count)
        return Status::Success;
    dirty = really_dirty = false;
    Widget** item = children;
    for (Container_size_t i = count; i > 0; i--, item++)
        (*item)->Paint();
    return Status::Success;
}


Container_size_t Container::increment_index(Container_size_t i)
{
    if (++i >= count)
        i = 0;
    return i;
}


Container_size_t Container::decrement_index(Container_size_t i)
{
    if (!i)
        i = count;
    return i--;
}


void Container::BeginUpdate()
{
    updating = true;
    begin_update();
}


void Container::begin_update()
{
    /* Do nothing */
}


void Container::EndUpdate()
{
    end_update();
    updating = false;
}


void Container::end_update()
{
    Layout();
}


Status Container::FocusNext()
{
    if (!count)
        return Status::Failure;
    if (!hasFocus)
        return Status::Failure;
    Container_size_t index = active_index;
    if (children[index]->Unfocus() != Status::Success)
        return Status::Failure;
    while (++index < count)
        if (children[index]->Focus() == Status::Success)
        {
            active_index = index;
            return Status::Success;
        }
    children[active_index]->Focus();
    return Status::Failure;
}


Status Container::FocusNextCircular()
{
    if (!count)
        return Status::Failure;
    if (!hasFocus)
        return Status::Failure;
    Container_size_t index = active_index;
    if (children[active_index]->Unfocus() != Status::Success)
        return Status::Failure;
    while ((index = increment_index(index)) != active_index)
        if (children[index]->Focus() == Status::Success)
        {
            active_index = index;
            return Status::Success;
        }
    children[active_index]->Focus();
    return Status::Failure;
}


Status Container::FocusPrevious()
{
    if (!count)
        return Status::Failure;
    if (!hasFocus)
        return Status::Failure;
    if (active_index == 0)
        return Status::Failure;
    Container_size_t index = active_index;
    if (children[active_index]->Unfocus() != Status::Success)
        return Status::Failure;
    while (index-- != 0)
        if (children[index]->Focus() == Status::Success)
        {
            active_index = index;
            return Status::Success;
        }
    children[active_index]->Focus();
    return Status::Failure;
}


Status Container::FocusPreviousCircular()
{
    if (!count)
        return Status::Failure;
    if (!hasFocus)
        return Status::Failure;
    Container_size_t index = active_index;
    if (children[active_index]->Unfocus() != Status::Success)
        return Status::Failure;
    while ((index = decrement_index(index)) != active_index)
        if (children[index]->Focus() == Status::Success)
        {
            active_index = index;
            return Status::Success;
        }
    children[active_index]->Focus();
    return Status::Failure;
}


Status Container::FocusFirst()
{
    if (!count)
        return Status::Failure;
    if (!hasFocus)
        return Status::Failure;
    if (children[active_index]->Unfocus() != Status::Success)
        return Status::Failure;
    for (Container_size_t index = 0; index < count; index++)
        if (children[index]->Focus() == Status::Success)
        {
            active_index = index;
            return Status::Success;
        }
    // This should never happen.
    //children[active_index]->Focus();
    return Status::Failure;
}


Status Container::FocusLast()
{
    if (!count)
        return Status::Failure;
    if (!hasFocus)
        return Status::Failure;
    if (children[active_index]->Unfocus() != Status::Success)
        return Status::Failure;
    Container_size_t index = count;
    while (index--)
        if (children[index]->Focus() == Status::Success)
        {
            active_index = index;
            return Status::Success;
        }
    // This should never happen.
    //children[active_index]->Focus();
    return Status::Failure;
}


Widget& Container::Get(Container_size_t index) const
{
    return *children[index];
}


/*Widget*& Container::operator[](Container_size_t index) const
{
    return children[index];
}*/


void Container::Replace(Container_size_t index, Widget& item)
{
    delete children[index];
    children[index] = &item;
    if (!updating)
        Layout();
}


Widget**  Container::enlarge()
{
    Container_size_t newsize = EMBIGGEN(size);
    if (newsize < size)
        newsize = -1;
    if (newsize == size)
        return nullptr;
    Widget** resized = new Widget*[newsize];
    if (resized == nullptr)
        return nullptr;
    memcpy(resized, children, sizeof(Widget*) * count);
    delete[] children;
    size = newsize;
    return children = resized;
}


Status Container::Add(Widget& widget)
{
    if (count >= size)
        if (!enlarge())
            return Status::Failure;
    children[count++] = &widget;
    widget.parent = this;
    if (!updating)
        Layout();
    return Status::Success;
}


Status Container::Insert(Container_size_t index, Widget& widget)
{
    if (count >= size)
        if (!enlarge())
            return Status::Failure;
    memmove(children + index + 1, children + index, count++ - index);
    children[index] = &widget;
    if (!updating)
        Layout();
    return Status::Success;
}


Widget& Container::Leak(Container_size_t index)
{
    Widget* widget = children[index];
    memmove(children + index, children + index + 1, --count - index);
    if (!updating)
        Layout();
    return *widget;
}


void Container::Delete(Container_size_t index)
{
    delete &Leak(index);
}
