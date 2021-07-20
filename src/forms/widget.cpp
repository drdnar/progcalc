#include "widget.h"
#include "gui.h"
#include "style.h"
#include "cursorblinker.h"
#include "inlineoptimizations.h"

using namespace Forms;


Widget::Widget()
{
    //
}


Widget::Widget(Widget_def* Template, Widget* Parent, Widget_def** Next)
 : definition { Template }, parent { Parent }, dirty { true }
{
    Widget_def* next = Template->TypeDescriptor->GetNextWidget(Template);
    if (Next)
        *Next = next;
    style = Style::constructify(next, getStylePointer(), Next);
}


Widget::~Widget()
{
    if (!style_not_deletable)
        delete style;
}


Widget_def* Widget::GetNextItem(Widget_def* item, size_t size) 
{
    if (item)
        return Style::GetNextItem((Widget_def*)((unsigned char*)item + size));
    else
        return nullptr;
}


Status Widget::MoveTo(x_t xx, y_t yy)
{
    x = xx;
    y = yy;
    return Status::Success;
}


Status Widget::SetWidth(x_t newwidth)
{
    if (newwidth < min_width)
        return Status::Failure;
    dirty = true;
    width = newwidth;
    return Status::Success;
}


Status Widget::SetHeight(y_t newheight)
{
    if (newheight < min_height)
        return Status::Failure;
    dirty = true;
    height = newheight;
    return Status::Success;
}


Status Widget::SetSize(x_t newwidth, y_t newheight)
{
    if (newwidth < min_width || newheight < min_height)
        return Status::Failure;
    x_t oldwidth = width;
    if (SetWidth(newwidth) != Status::Failure)
    {
        if (SetHeight(newheight) != Status::Failure)
            return Status::Success;
        else
            SetWidth(oldwidth);
    }
    return Status::Failure;
}


Status Widget::Focus()
{
    /*if (!(Enable() != Status::Success))
        return Status::Failure;
    if (!(Show() != Status::Success))
        return Status::Failure;*/
//    if (hidden || disabled)
//        return Status::Failure;
    hasFocus = true;
    CursorBlinker::Restart();
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


bool Widget::SendInput([[maybe_unused]] Message& message)
{
    return false;
}


void Widget::SetDirty()
{
    if (dirty)
        return;
    dirty = true;
    if (parent && !parent->dirty)
        parent->SetDirty();
}


void Widget::setWindow()
{
    fontlib_SetWindow(x, y, width, height);
}


Style* Widget::getStylePointer() const
{
    if (style)
        return style;
    if (parent)
        return parent->getStylePointer();
    GUI& gui = GUI::GetInstance();
    Container* c = gui.GetActiveDialog();
    if (c)
        return c->getStylePointer();
    return gui.style;
}


Style& Widget::ChangeStyle()
{
    if (style)
        return *style;
    style = new Style(*getStylePointer());
    return *style;
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
    if (!(next->TypeDescriptor->Flags & WIDGET_FLAG_NON_INSTANTIABLE))
        return false;
    if (style) // This . . . shouldn't be able to happen?
        delete style;
    //style = Style::constructify(next, getStylePointer(), next);
    if (style)
        return true;
    return false;
}
