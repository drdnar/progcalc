#include "checkbox.h"
#include "calc1252.h"
#include <fontlibc.h>
#include "textmanager.h"
#include "style.h"
#include "cursorblinker.h"

using namespace Forms;


Widget_def* Checkbox::GetNextItem(Widget_def* Template)
{
    return Widget::GetNextItem(Template, sizeof(Checkbox_def));
}


Widget* Checkbox::forms_ctor(Widget_def* Template, Widget* parent, Widget_def** next)
{
    return new Checkbox(Template, parent, next);
}


Checkbox::Checkbox(Widget_def* Template, Widget* parent, Widget_def** next)
 : Widget(Template, parent, next)
{
    text = ((Checkbox_def*)Template)->Text;
    variable = ((Checkbox_def*)Template)->Variable;
    GetStyle().ActivateFont();
    height = fontlib_GetCurrentFontHeight();
    width = fontlib_GetStringWidth(text)
        + fontlib_GetStringWidth(CALC1252_CURSOR_RIGHT " " CALC1252_RADIO_UNCHECKED " " CALC1252_CURSOR_LEFT);
}


extern "C" const Widget_desc Checkbox_desc
{
    ID::WIDGET_ID_Checkbox,
    WIDGET_FLAG_NONE,
    &Checkbox::forms_ctor,
    &Checkbox::GetNextItem
};


Status Checkbox::Paint()
{
    if (!dirty)
        return Status::Success;
    dirty = false;
    char ch;
    GetStyle().ActivateFont();
    fontlib_SetCursorPosition(x, y);
    if (hasFocus && CursorBlinker::IsShowing())
        ch = CALC1252_CURSOR_RIGHT_CHAR;
    else
        ch = CALC1252_CURSOR_BLANK_CHAR;
    fontlib_DrawGlyph(ch);
    if (*variable)
        ch = CALC1252_RADIO_CHECKED_CHAR;
    else
        ch = CALC1252_RADIO_UNCHECKED_CHAR;
    fontlib_DrawGlyph(ch);
    if (hasFocus && CursorBlinker::IsShowing())
        ch = CALC1252_CURSOR_LEFT_CHAR;
    else
        ch = CALC1252_CURSOR_BLANK_CHAR;
    fontlib_DrawGlyph(ch);
    fontlib_DrawString(text);
    fontlib_DrawGlyph(' ');
    return Status::Success;
}


bool Checkbox::SendInput(Message& message)
{
    if (message.Id == MESSAGE_BLINK)
        SetDirty();
    else if (message.Id == MESSAGE_KEY && (message.ExtendedCode == sk_Enter || message.ExtendedCode == sk_2nd))
    {
        dirty = true;
        *variable = !*variable;
        Paint();
        return true;
    }
    return false;
}
