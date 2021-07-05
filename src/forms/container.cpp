#include "container.h"
#include <string.h>

using namespace Forms;

/**
 * Returns a somewhat larger number, using a spurious coinage.
 */
#define EMBIGGEN(x) (((x) * 3 + 1) / 2)

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
    _children = new Widget*[size];
}


Container::Container(Container_def* Template, Widget_def** Next)
{
    Widget_def* next = &Template->FirstChild;
    size = Template->ChildCount;
    _children = new Widget*[size];
    Widget** child = _children;
    count = Template->ChildCount;
    for (unsigned char i = count; i > 0; i--, child++)
        *child = next->TypeDescriptor->ctor(next, this, &next);
    if (Next != nullptr)
        *Next = next;
}


Container::~Container()
{
    if (!_children)
        return;
    Widget** child = _children;
    for (Container_size_t i = count; i > 0; i--, child++)
        delete *child;
    delete[] _children;
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
    Widget** item = _children;
    for (Container_size_t i = count; i > 0; i--, item++)
        (*item)->MoveTo((*item)->x + dx, (*item)->y + dy);
    return Status::Success; 
}


Status Container::Focus()
{
    if (!count)
        return Status::Failure;
    Status r = _children[_active_index]->Focus();
    hasFocus = r == Status::Success;
    return r;
}


Status Container::Unfocus()
{
    if (!count)
        return Status::Success;
    Status r = _children[_active_index]->Unfocus();
    hasFocus = r != Status::Success;
    return r;
}


bool Container::SendInput(Message& message)
{
    if (!count)
        return _children[_active_index]->SendInput(message);
    return false;
}


bool Container::Broadcast(Message& message)
{
    if (!count)
        return false;
    Widget** item = _children;
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
    if (!count)
        return;
    Widget** item = _children;
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
    dirty = false;
    Widget** item = _children;
    for (Container_size_t i = count; i > 0; i--, item++)
        (*item)->Paint();
    return Status::Success;
}


Container_size_t Container::_increment_index(Container_size_t i)
{
    if (++i >= count)
        i = 0;
    return i;
}


Container_size_t Container::_decrement_index(Container_size_t i)
{
    if (!i)
        i = count;
    return i--;
}


void Container::BeginUpdate()
{
    _updating = true;
    _begin_update();
}


void Container::_begin_update()
{
    /* Do nothing */
}


void Container::EndUpdate()
{
    _end_update();
    _updating = false;
}


void Container::_end_update()
{
    Layout();
}


Status Container::FocusNext()
{
    if (!count)
        return Status::Failure;
    if (!hasFocus)
        return Status::Failure;
    Container_size_t index = _active_index;
    if (_children[_active_index]->Unfocus() != Status::Success)
        return Status::Failure;
    while ((index = _increment_index(index)) != _active_index)
        if (_children[index]->Focus() == Status::Success)
        {
            _active_index = index;
            return Status::Success;
        }
    /*if (_children[_active_index]->Focus() != Status::Success) // ? ? ?
        _active_index = 0;*/
    _children[_active_index]->Focus();
    return Status::Failure;
}


Status Container::FocusPrevious()
{
    if (!count)
        return Status::Failure;
    if (!hasFocus)
        return Status::Failure;
    Container_size_t index = _active_index;
    if (_children[_active_index]->Unfocus() != Status::Success)
        return Status::Failure;
    while ((index = _decrement_index(index)) != _active_index)
        if (_children[index]->Focus() == Status::Success)
        {
            _active_index = index;
            return Status::Success;
        }
    /*if (_children[_active_index]->Focus() != Status::Success) // ? ? ?
        _active_index = 0;*/
    _children[_active_index]->Focus();
    return Status::Failure;
}


Widget& Container::Get(Container_size_t index) const
{
    return *_children[index];
}


/*Widget*& Container::operator[](Container_size_t index) const
{
    return _children[index];
}*/


void Container::Replace(Container_size_t index, Widget& item)
{
    delete _children[index];
    _children[index] = &item;
    if (!_updating)
        Layout();
}


Widget**  Container::_enlarge()
{
    Container_size_t newsize = EMBIGGEN(size);
    if (newsize < size)
        newsize = -1;
    if (newsize == size)
        return nullptr;
    Widget** resized = new Widget*[newsize];
    if (resized == nullptr)
        return nullptr;
    memcpy(resized, _children, sizeof(Widget*) * count);
    delete[] _children;
    size = newsize;
    return _children = resized;
}


Status Container::Add(Widget& widget)
{
    if (count >= size)
        if (!_enlarge())
            return Status::Failure;
    _children[count++] = &widget;
    widget.parent = this;
    if (!_updating)
        Layout();
    return Status::Success;
}


Status Container::Insert(Container_size_t index, Widget& widget)
{
    if (count >= size)
        if (!_enlarge())
            return Status::Failure;
    memmove(_children + index + 1, _children + index, count++ - index);
    _children[index] = &widget;
    if (!_updating)
        Layout();
    return Status::Success;
}


Widget& Container::Leak(Container_size_t index)
{
    Widget* widget = _children[index];
    memmove(_children + index, _children + index + 1, --count - index);
    if (!_updating)
        Layout();
    return *widget;
}


void Container::Delete(Container_size_t index)
{
    delete &Leak(index);
}
