#include "checkbox.h"
#include "calc1252.h"
#include <fontlibc.h>
#include "textmanager.h"
#include "style.h"

using namespace Forms;


static Widget_def* GetNextItem(Widget_def* Template)
{
    return (Widget_def*)((Checkbox_def*)Template + 1);
}


Widget* Forms::Checkbox_ctor(Widget_def* Template, Widget* parent, Widget_def** next)
{
    Checkbox* widget = new Checkbox();
    widget->definition = Template;
    widget->parent = parent;
    widget->text = ((Checkbox_def*)Template)->Text;
    widget->variable = ((Checkbox_def*)Template)->Variable;
    widget->GetStyle().ActivateFont();
    widget->height = fontlib_GetCurrentFontHeight();
    widget->width = fontlib_GetStringWidth(widget->text)
        + fontlib_GetStringWidth(CALC1252_CURSOR_RIGHT " " CALC1252_RADIO_UNCHECKED "   " CALC1252_CURSOR_LEFT);
    if (next != NULL)
        *next = (Widget_def*)((Checkbox_def*)Template + 1);
    return widget;
}


extern "C" const Widget_desc Checkbox_desc
{
    ID::WIDGET_ID_Checkbox,
    WIDGET_FLAG_NONE,
    &Forms::Checkbox_ctor,
    &GetNextItem
};


Status Checkbox::Focus()
{
    SetDirty();
    return Widget::Focus();
};


Status Checkbox::Unfocus()
{
    SetDirty();
    return Widget::Focus();
};


Status Checkbox::Paint()
{
    if (!dirty)
        return Status::Success;
    dirty = false;
    char ch;
    GetStyle().ActivateFont();
    fontlib_SetCursorPosition(x, y);
    if (hasFocus)
        ch = CALC1252_CURSOR_RIGHT_CHAR;
    else
        ch = CALC1252_CURSOR_BLANK_CHAR;
    fontlib_DrawGlyph(ch);
    fontlib_DrawGlyph(' ');
    if (*variable)
        ch = CALC1252_RADIO_CHECKED_CHAR;
    else
        ch = CALC1252_RADIO_UNCHECKED_CHAR;
    fontlib_DrawGlyph(ch);
    fontlib_DrawGlyph(' ');
    fontlib_DrawString(text);
    fontlib_DrawGlyph(' ');
    if (hasFocus)
        ch = CALC1252_CURSOR_LEFT_CHAR;
    else
        ch = CALC1252_CURSOR_BLANK_CHAR;
    fontlib_DrawGlyph(ch);
    return Status::Success;
}


bool Checkbox::SendInput(Message& message)
{
    if (message.Id == MESSAGE_KEY && (message.ExtendedCode == sk_Enter || message.ExtendedCode == sk_2nd))
    {
        dirty = true;
        *variable = !*variable;
        Paint();
        return true;
    }
    return false;
}
