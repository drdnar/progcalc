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


void Forms::Container_ctor(Container_def* Template, Container& self, Widget_def** Next)
{
    Widget_def* next = &Template->FirstChild;
    if (self._size == 0)
        self._size = EMBIGGEN(Template->ChildCount);
    self._children = new Widget*[self._size];
    Widget** child = self._children;
    if (self._children)
    {
        self._count = Template->ChildCount;
        for (unsigned char i = self._count; i > 0; i--, child++)
            *child = next->TypeDescriptor->ctor(next, &self, &next);
    }
    else
    {
        /* Out of memory, so, uh . . . skip over the child list, I guess. */
        next = Container::GetNextItem(Template);
        self._size = 0;
        self._count = 0;
    }
    if (Next != nullptr)
        *Next = next;
}


Container::~Container()
{
    if (!_children)
        return;
    Widget** child = _children;
    for (Container_size_t i = _count; i > 0; i--, child++)
        delete *child;
    delete[] _children;
}


Status Container::MoveTo(uint24_t x, uint8_t y)
{
    /* Signed or unsigned, doesn't really matter here, just let it overflow. */
    uint24_t dx = x - _x;
    uint24_t dy = y - _y;
    _x = x;
    _y = y;
    if (!_count)
        return Status::Success;
    Widget** item = _children;
    for (Container_size_t i = _count; i > 0; i--, item++)
        (*item)->MoveTo((*item)->_x + dx, (*item)->_y + dy);
    return Status::Success; 
}


Status Container::Focus(void)
{
    if (!_count)
        return Status::Failure;
    Status r = _children[_active_index]->Focus();
    _hasFocus = r == Status::Success;
    return r;
}


Status Container::Unfocus(void)
{
    if (!_count)
        return Status::Success;
    Status r = _children[_active_index]->Unfocus();
    _hasFocus = r != Status::Success;
    return r;
}


WidgetMessage Container::SendInput(WidgetMessage message)
{
    if (!_count)
        return _children[_active_index]->SendInput(message);
    return 0;
}


Status Container::Paint(void)
{
    if (!_count)
        return Status::Success;
    Widget** item = _children;
    for (Container_size_t i = _count; i > 0; i--, item++)
        (*item)->Paint();
    return Status::Success;
}


Container_size_t Container::_increment_index(Container_size_t i)
{
    if (++i >= _count)
        i = 0;
    return i;
}


Container_size_t Container::_decrement_index(Container_size_t i)
{
    if (!i)
        i = _count;
    return i--;
}


void Container::BeginUpdate(void)
{
    _updating = true;
    _begin_update();
}


void Container::_begin_update(void)
{
    /* Do nothing */
}


void Container::EndUpdate(void)
{
    _end_update();
    _updating = false;
}


void Container::_end_update(void)
{
    Paint();
}


Status Container::FocusNext()
{
    if (!_count)
        return Status::Failure;
    if (!_hasFocus)
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
    if (!_count)
        return Status::Failure;
    if (!_hasFocus)
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
}


Widget**  Container::_enlarge(void)
{
    Container_size_t newsize = EMBIGGEN(_size);
    if (newsize < _size)
        newsize = -1;
    if (newsize == _size)
        return nullptr;
    Widget** resized = new Widget*[newsize];
    if (resized == nullptr)
        return nullptr;
    memcpy(resized, _children, sizeof(Widget*) * _count);
    delete[] _children;
    _size = newsize;
    return _children = resized;
}


Status Container::Add(Widget& widget)
{
    if (_count >= _size)
        if (!_enlarge())
            return Status::Failure;
    _children[_count++] = &widget;
    return Status::Success;
}


Status Container::Insert(Container_size_t index, Widget& widget)
{
    if (_count >= _size)
        if (!_enlarge())
            return Status::Failure;
    memmove(_children + index + 1, _children + index, _count++ - index);
    _children[index] = &widget;
    return Status::Success;
}


Widget& Container::Leak(Container_size_t index)
{
    Widget* widget = _children[index];
    memmove(_children + index, _children + index + 1, --_count - index);
    return *widget;
}


void Container::Delete(Container_size_t index)
{
    delete &Leak(index);
}
