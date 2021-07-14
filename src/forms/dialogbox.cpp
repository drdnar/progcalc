#include "dialogbox.h"
#include "drawbox.h"
#include <graphx.h>
#include "inlineoptimizations.h"
#include "gui.h"

using namespace Forms;


Widget_def* DialogBox::GetNextItem(Widget_def* Template)
{
    if (Template == nullptr)
        return nullptr;
    return Container::GetNextItem(&((DialogBox_def*)Template)->Contents);
}


Widget* DialogBox::form_ctor(Widget_def* Template, Widget* parent, Widget_def** Next)
{
    return new DialogBox(Template, parent, Next);
}


DialogBox::DialogBox(Widget_def* Template, Widget* Parent, Widget_def** next)
 : Container(&((DialogBox_def*)Template)->Contents, Parent, next)
{
    width = min_width = ((DialogBox_def*)Template)->MinimumWidth;
    height = min_height = ((DialogBox_def*)Template)->MinimumHeight;
    Widget** widget = &children[0];
    header = dynamic_cast<Container*>(*widget++);
    body = dynamic_cast<Container*>(*widget++);
    footer = dynamic_cast<Container*>(*widget++);
    auto hheight = header->GetHeight();
    auto fheight = footer->GetHeight();
    auto innerwidth = width - DrawBox_HorizontalPadding;
    header->SetWidth(innerwidth);
    footer->SetWidth(innerwidth);
    body->SetSize(innerwidth, height - hheight - fheight - DrawBox_VerticalPadding);
    active_index = 1;
}


extern "C" const Widget_desc DialogBox_desc
{
    ID::WIDGET_ID_DialogBox,
    WIDGET_FLAG_CONTAINER,
    &DialogBox::form_ctor,
    &DialogBox::GetNextItem
};


void DialogBox::Layout()
{
    dirty = true;
    auto& gui = GUI::GetInstance();
    auto xb = gui.GetX();
    auto yb = gui.GetY();
    auto w = gui.GetWidth();
    auto h = gui.GetHeight();
    x = w / 2 - width / 2 + xb;
    y = h / 2 - height / 2 + yb;
    x_t xx = x + DrawBox_LeftPadding;
    y_t yy = y + DrawBox_TopPadding;
    Widget** child = children;
    for (Container_size_t i = count; i > 0; i--, child++)
    {
        (*child)->MoveTo(xx, yy);
        Container* container = dynamic_cast<Container*>(*child);
        if (container)
            container->Layout();
        yy += (*child)->GetHeight();
    }
}


bool DialogBox::SendInput(Message& message)
{
    if (Container::SendInput(message))
        return true;
    if (message.Id == MESSAGE_GUI_EVENT)
    {
        switch (message.ExtendedCode)
        {
            case GUI_EVENT_UP:
            case GUI_EVENT_PAGE_UP:
                if (active_index == 1)
                    return false;
                active_index = 1;
                footer->Unfocus();
                body->Focus();
                return true;
            case GUI_EVENT_DOWN:
            case GUI_EVENT_PAGE_DOWN:
                if (active_index == 2)
                    return false;
                active_index = 2;
                body->Unfocus();
                footer->Focus();
                return true;
            default:
                return false;
        }
    }
    return false;
}


Status DialogBox::Paint()
{
    if (!dirty)
        return Status::Success;
    DrawBox(x, y, width, height, GetStyle());
    if (really_dirty)
    {
        gfx_SetColor(GetStyle().GetBackgroundColor());
        gfx_FillRectangle_NoClip(x + DrawBox_LeftPadding, y + DrawBox_TopPadding, width - DrawBox_HorizontalPadding, height - DrawBox_VerticalPadding);
    }
    return Container::Paint();
}
