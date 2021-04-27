#include "checkbox.h"
#include "../calc1252.h"
#include <fontlibc.h>
#include "../style.h"
#include "../ui.h"

using namespace Forms;


static Widget_def* GetNextItem(Widget_def* Template)
{
    return (Widget_def*)((Checkbox_def*)Template + 1);
}


Widget* Forms::Checkbox_ctor(Widget_def* Template, Widget* parent, Widget_def** next)
{
    Checkbox* widget = new Checkbox();
    widget->_definition = Template;
    widget->_parent = parent;
    widget->_font = ((Checkbox_def*)Template)->Font;
    widget->_text = ((Checkbox_def*)Template)->Text;
    widget->_variable = ((Checkbox_def*)Template)->Variable;
    Style_SetFont(widget->_font);
    widget->_height = fontlib_GetCurrentFontHeight();
    widget->_width = fontlib_GetStringWidth(widget->_text)
        + fontlib_GetStringWidth(CALC1252_CURSOR_RIGHT " " CALC1252_RADIO_UNCHECKED "   " CALC1252_CURSOR_LEFT);
    if (next != NULL)
        *next = (Widget_def*)((Checkbox_def*)Template + 1);
    return widget;
}


extern "C" const Widget_desc Checkbox_desc
{
    ID::Checkbox,
    &Forms::Checkbox_ctor,
    &GetNextItem
};


Status Checkbox::Focus(void)
{
    _hasFocus = true;
    Paint();
    return Status::Success;
};


Status Checkbox::Unfocus(void)
{
    _hasFocus = false;
    Paint();
    return Status::Success;
};


Status Checkbox::Paint()
{
    char ch;
    Style_SetFont(_font);
    fontlib_SetCursorPosition(_x, _y);
    if (_hasFocus)
        ch = CALC1252_CURSOR_RIGHT_CHAR;
    else
        ch = CALC1252_CURSOR_BLANK_CHAR;
    fontlib_DrawGlyph(ch);
    fontlib_DrawGlyph(' ');
    if (*_variable)
        ch = CALC1252_RADIO_CHECKED_CHAR;
    else
        ch = CALC1252_RADIO_UNCHECKED_CHAR;
    fontlib_DrawGlyph(ch);
    fontlib_DrawGlyph(' ');
    fontlib_DrawString(_text);
    fontlib_DrawGlyph(' ');
    if (_hasFocus)
        ch = CALC1252_CURSOR_LEFT_CHAR;
    else
        ch = CALC1252_CURSOR_BLANK_CHAR;
    fontlib_DrawGlyph(ch);
    return Status::Success;
}


WidgetMessage Checkbox::SendInput(WidgetMessage messageId)
{
    if (messageId == sk_Enter || messageId == sk_2nd)
    {
        *_variable = !*_variable;
        Paint();
        return 1;
    }
    return 0;
}
