#include "widget.h"

using namespace Forms;


Status Widget::MoveTo(uint24_t x, uint8_t y)
{
    _x = x;
    _y = y;
    return Status::Success;
}


uint24_t Widget::GetX(void) const
{
    return _x;
}


uint8_t Widget::GetY(void) const
{
    return _y;
}


Status Widget::SetSize(uint24_t width, uint8_t height)
{
    if (width < _min_width || height < _min_height)
        return Status::Failure;
    _width = width;
    _height = height;
    return Status::Success;
}


uint24_t Widget::GetWidth(void) const
{
    return _width;
}


uint8_t Widget::GetHeight(void) const
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
