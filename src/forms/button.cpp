#include "button.h"
#include <fontlibc.h>
#include "textmanager.h"
#include "ignorewarning.h"
#include "ez80.h"
#include "style.h"
#include "drawbox.h"
#include "calc1252.h"
#include "cursorblinker.h"

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
    width = min_width = ((Button_def*)Template)->MinimumWidth;
    GetStyle().ActivateFont();
    textWidth = fontlib_GetStringWidth(GetText());
    textWidth += fontlib_GetGlyphWidth(CALC1252_CURSOR_LEFT_CHAR) * 2;
    SetWidth(textWidth + DrawBox_HorizontalPadding);
    SetHeight(fontlib_GetCurrentFontHeight() + DrawBox_VerticalPadding);
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
//    if (!hasFocus)
//        return false;
    if (message.Id == MESSAGE_BLINK)
        SetDirty();
    else if (message.Id == MESSAGE_KEY && message.ExtendedCode == sk_Enter)
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
    gfx_SetColor(GetStyle().GetBackgroundColor());
    gfx_FillRectangle_NoClip(x + DrawBox_LeftPadding, ty, leftpad, height - DrawBox_VerticalPadding);
    GetStyle().ActivateFont();
    fontlib_SetCursorPosition(x + DrawBox_LeftPadding + leftpad, ty);
    if (hasFocus && CursorBlinker::IsShowing())
        fontlib_DrawGlyph(CALC1252_CURSOR_RIGHT_CHAR);
    else
        fontlib_DrawGlyph(CALC1252_CURSOR_BLANK_CHAR);
        //fontlib_DrawGlyph(CALC1252_CURSOR_SHADED_CHAR);
    fontlib_DrawString(GetText());
    if (hasFocus && CursorBlinker::IsShowing())
        fontlib_DrawGlyph(CALC1252_CURSOR_LEFT_CHAR);
    else
        fontlib_DrawGlyph(CALC1252_CURSOR_BLANK_CHAR);
        //fontlib_DrawGlyph(CALC1252_CURSOR_SHADED_CHAR);
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

