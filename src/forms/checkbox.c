#define WIDGET Checkbox
#define TYPEID CHECKBOX
#include "widget.inc.h"
#include "checkbox.h"
#include "../calc1252.h"
#include <fontlibc.h>
#include "../ui.h"

static const WIDGET_vtable vtable =
{
    /* Widget */
    {
        sizeof(WIDGET_vtable),
        false,
        &GetNextItem,
        &WIDGET_ctor,
        &GenericWidget_dtor,
        &GenericWidget_MoveTo,
        &Paint,
        &GenericWidget_Focus,
        &GenericWidget_Unfocus,
        &SendInput
    }
};


static Widget_def* GetNextItem(const Widget_def* Template)
{
    return (Widget_def*)((WIDGET_t*)Template + 1);
}


Widget_t* WIDGET_ctor(const Widget_def* Template, Widget_t* parent, Widget_def** next)
{
    WIDGET_t* widget = (WIDGET_t*)malloc(sizeof(WIDGET_t));
    widget->Widget.TypeId = TYPEID;
    widget->Widget.vtable = (Widget_vtable*)&vtable;
    widget->Widget.Definition = Template;
    widget->Widget.Parent = parent;
    Style_SetFont(((WIDGET_def*)Template)->Font);
    widget->Widget.Height = fontlib_GetCurrentFontHeight();
    widget->Widget.Width = fontlib_GetStringWidth(((WIDGET_def*)Template)->Text)
        + fontlib_GetStringWidth(CALC1252_CURSOR_RIGHT " " CALC1252_RADIO_UNCHECKED "  " CALC1252_CURSOR_LEFT);
    if (next != NULL)
        *next = (Widget_def*)((WIDGET_t*)Template + 1);
    return (Widget_t*)widget;
}


static int24_t Paint(Widget_t* self)
{
    char ch;
    Style_SetFont(definition->Font);
    fontlib_SetCursorPosition(self->X, self->Y);
    if (this->Widget.HasFocus)
        ch = CALC1252_CURSOR_RIGHT_CHAR;
    else
        ch = CALC1252_CURSOR_BLANK_CHAR;
    fontlib_DrawGlyph(ch);
    fontlib_DrawGlyph(' ');
    if (*definition->Variable)
        ch = CALC1252_RADIO_CHECKED_CHAR;
    else
        ch = CALC1252_RADIO_UNCHECKED_CHAR;
    fontlib_DrawGlyph(ch);
    fontlib_DrawString(definition->Text);
    fontlib_DrawGlyph(' ');
    if (this->Widget.HasFocus)
        ch = CALC1252_CURSOR_LEFT_CHAR;
    else
        ch = CALC1252_CURSOR_BLANK_CHAR;
    fontlib_DrawGlyph(ch);
    return 0;
}


static int24_t SendInput(Widget_t* self, int24_t messageId)
{
    if (messageId == sk_Enter || messageId == sk_2nd)
    {
        *definition->Variable = !*definition->Variable;
        Paint(self);
        return 1;
    }
    return 0;
    
}
