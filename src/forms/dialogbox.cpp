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
    definition = Template;
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
    FocusFirst();
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
    if (!loaded)
    {
        loaded = true;
        auto callback = ((DialogBox_def*)definition)->OnLoad;
        if (callback)
           callback(*this);
    }
}


bool DialogBox::SendInput(Message& message)
{
    if (message.Id == MESSAGE_GUI_EVENT)
    {
        auto customHandler = ((DialogBox_def*)definition)->OnGuiEvent;
        if (customHandler)
            if (customHandler(*this, message.ExtendedCode))
                return true;
        switch (message.ExtendedCode)
        {
            case GUI_EVENT_PAGE_UP:
                if (FocusFirst() != Status::Failure)
                {
                    if (((Container*)children[active_index])->FocusFirst() != Status::Failure)
                        return true;
                }
                break;
            case GUI_EVENT_PAGE_DOWN:
                if (FocusLast() != Status::Failure)
                {
                    if (((Container*)children[active_index])->FocusFirst() != Status::Failure)
                        return true;
                }
                break;
            case GUI_EVENT_OK:
            case GUI_EVENT_CANCEL:
                MessageLoop::EnqueueMessage({ .Id = MESSAGE_GUI_MODAL_END, .ExtendedCode = MESSAGE_NONE });
                return true;
        }
    }
    if (Container::SendInput(message))
        return true;
    if (message.Id == MESSAGE_GUI_EVENT)
    {
        switch (message.ExtendedCode)
        {
            case GUI_EVENT_UP:
                if (FocusPrevious() != Status::Failure)
                    return true;
                break;
            case GUI_EVENT_DOWN:
                if (FocusNext() != Status::Failure)
                    return true;
                break;
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
