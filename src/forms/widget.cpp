#include "widget.h"

using namespace Forms;


Widget::~Widget()
{
    // Do nothing.
}


Status Widget::MoveTo(x_t xx, y_t yy)
{
    x = xx;
    y = yy;
    return Status::Success;
}


Status Widget::SetSize(x_t newwidth, y_t newheight)
{
    if (newwidth < min_width || newheight < min_height)
        return Status::Failure;
    width = newwidth;
    height = newheight;
    return Status::Success;
}


Status Widget::Focus()
{
    hasFocus = true;
    Paint();
    return Status::Success;
}
        

Status Widget::Unfocus()
{
    hasFocus = false;
    Paint();
    return Status::Success;
}


Widget* Widget::GetParent()
{
    return parent;
}


Status Widget::Disable()
{
    disabled = true;
    return Status::Success;
}


Status Widget::Enable()
{
    disabled = false;
    return Status::Success;
}


Status Widget::Hide()
{
    hidden = true;
    return Status::Success;
}


Status Widget::Show()
{
    hidden = false;
    return Status::Success;
}


void Widget::SetDirty()
{
    dirty = true;
    if (parent && !parent->dirty)
        parent->SetDirty();
}
