#include "widget.h"

using namespace Forms;


Widget::~Widget()
{
    // Do nothing.
}


Status Widget::MoveTo(x_t x, y_t y)
{
    _x = x;
    _y = y;
    return Status::Success;
}


x_t Widget::GetX(void) const
{
    return _x;
}


y_t Widget::GetY(void) const
{
    return _y;
}


Status Widget::SetSize(x_t width, y_t height)
{
    if (width < _min_width || height < _min_height)
        return Status::Failure;
    _width = width;
    _height = height;
    return Status::Success;
}


x_t Widget::GetWidth(void) const
{
    return _width;
}


y_t Widget::GetHeight(void) const
{
    return _height;
}


Status Widget::Focus(void)
{
    _hasFocus = true;
    Paint();
    return Status::Success;
}
        

Status Widget::Unfocus(void)
{
    _hasFocus = false;
    Paint();
    return Status::Success;
}


bool Widget::HasFocus(void)
{
    return _hasFocus;
}


Widget* Widget::GetParent(void)
{
    return _parent;
}


bool Widget::IsDisabled(void)
{
    return _disabled;
}


Status Widget::Disable(void)
{
    _disabled = true;
    return Status::Success;
}


Status Widget::Enable(void)
{
    _disabled = false;
    return Status::Success;
}


bool Widget::IsHidden(void)
{
    return _hidden;
}


Status Widget::Hide(void)
{
    _hidden = true;
    return Status::Success;
}


Status Widget::Show(void)
{
    _hidden = false;
    return Status::Success;
}
