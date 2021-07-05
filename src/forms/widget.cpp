#include "widget.h"
#include "gui.h"
#include "style.h"

using namespace Forms;


Widget::Widget() : dirty { true }
{
    //
}

Widget::~Widget()
{
    if (!style_not_deletable)
        delete style;
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
    if (!(Enable() != Status::Success))
        return Status::Failure;
    if (!(Show() != Status::Success))
        return Status::Failure;
    hasFocus = true;
    SetDirty();
    return Status::Success;
}
        

Status Widget::Unfocus()
{
    hasFocus = false;
    SetDirty();
    return Status::Success;
}


Widget* Widget::GetParent()
{
    return parent;
}


Status Widget::Disable()
{
    Unfocus();
    disabled = true;
    SetDirty();
    return Status::Success;
}


Status Widget::Enable()
{

    Show();
    disabled = false;
    SetDirty();
    return Status::Success;
}


Status Widget::Hide()
{
    Unfocus();
    hidden = true;
    SetDirty();
    return Status::Success;
}


Status Widget::Show()
{
    hidden = false;
    SetDirty();
    return Status::Success;
}


void Widget::SetDirty()
{
    if (dirty)
        return;
    dirty = true;
    if (parent && !parent->dirty)
        parent->SetDirty();
}


Style* Widget::getStylePointer()
{
    if (style)
        return style;
    if (parent)
        return parent->getStylePointer();
    GUI& gui = GUI::GetInstance();
    Container* c;
    if (c = gui.GetActiveDialog())
        return c->getStylePointer();
    return gui.style;
}


Style& Widget::ChangeStyle()
{
    if (style)
        return *style;
    style = new Style(*getStylePointer());
}


void Widget::OverrideStyle(Style& newStyle)
{
    if (!style_not_deletable)
        delete style;
    style = new Style(newStyle);
}


bool Widget::loadStyle()
{
    Widget_desc* desc = definition->TypeDescriptor;
    Widget_def* next = desc->GetNextWidget(definition);
    if (!next)
        return false;
    if (!(next->TypeDescriptor->Flags & WIDGET_FLAG_STYLE_OVERRIDE))
        return false;
    if (style) // This . . . shouldn't be able to happen?
        delete style;
    style = Style::constructify( (FullStyleOverride_def*)next, getStylePointer() );
    if (style)
        return true;
    return false;
}
