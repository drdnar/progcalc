#include "button.h"
#include <fontlibc.h>
#include "textmanager.h"
#include "ignorewarning.h"
#include "ez80.h"
#include "style.h"
#include "drawbox.h"

using namespace Forms;


Widget_def* Button::GetNextItem(Widget_def* Template)
{
    return Widget::GetNextItem(Template, sizeof(Button_def));
}


Widget* Button::forms_ctor(Widget_def* Template, Widget* parent, Widget_def** next)
{
    return new Button(Template, parent, next);
}


Button::Button(Widget_def* Template, Widget* parent, Widget_def** next)
    : Widget(Template, parent, next)
{
    min_width = ((Button_def*)Template)->MinimumWidth;
    GetStyle().ActivateFont();
    SetSize(fontlib_GetStringWidth(GetText()) + DrawBox_HorizontalPadding, fontlib_GetCurrentFontHeight() + DrawBox_VerticalPadding);
}


extern "C" const Widget_desc Button_desc
{
    ID::WIDGET_ID_Button,
    WIDGET_FLAG_NONE,
    &Button::forms_ctor,
    &Button::GetNextItem
};


Button::~Button()
{
    // Do nothing
}


bool Button::SendInput(Message& message)
{
    if (message.Id != MESSAGE_KEY)
        return false;
    if (message.ExtendedCode == sk_Enter)
    {
        Press();
        return true;
    }
    return false;
}


Status Button::Paint()
{
    if (!dirty || hidden)
        return Status::Success;
    dirty = false;
    DrawBox(x, y, width, height);
    // Erase whitespace on left of text
    unsigned int leftpad = (width - DrawBox_HorizontalPadding) / 2 - textWidth / 2;
    y_t ty = y + DrawBox_TopPadding;
    gfx_FillRectangle_NoClip(x + DrawBox_LeftPadding, ty, leftpad, height - DrawBox_VerticalPadding);
    GetStyle().ActivateFont();
    fontlib_SetCursorPosition(x + DrawBox_LeftPadding + leftpad, ty);
    fontlib_DrawString(GetText());
    // Erase whitespace on right of text
    unsigned int xx = fontlib_GetCursorX();
    gfx_FillRectangle_NoClip(xx, ty, x + width - DrawBox_RightPadding - xx, height - DrawBox_VerticalPadding);
    return Status::Success;
}


void Button::Press()
{
    auto callback = ((Button_def*)definition)->OnPress;
    if (!callback)
        callback(*this);
}

