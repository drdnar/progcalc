#define WIDGET Checkbox
#include "widget.h"
#include "checkbox.h"
#include "../calc1252.h"
#include <fontlibc.h>
#include "../ui.h"

static const Widget_vtable vtable =
{
    sizeof(Widget_vtable),
    false,
    &GetNextItem,
    &ctor,
    &GenericWidget_dtor,
    &GenericWidget_MoveTo,
    /* Paint */
    &Paint,
    /* Focus */
    &GenericWidget_Focus,
    /* Unfocus */
    &GenericWidget_Unfocus,
    &SendInput
};


static Widget_def* GetNextItem(Widget_def* Template)
{
    return (Widget_def*)((Checkbox_t*)Template + 1);
}


static Widget_t* ctor(Widget_def* Template, Widget_t* parent)
{
    Checkbox_t* checkbox = (WIDGET_t*)malloc(sizeof(WIDGET_t));
    checkbox->GenericData.TypeId = CHECKBOX;
    checkbox->GenericData.vtable = &vtable;
    checkbox->GenericData.Definition = Template;
    checkbox->GenericData.Parent = parent;
    Style_SetFont(((WIDGET_def*)Template)->Font);
    checkbox->GenericData.Height = fontlib_GetCurrentFontHeight();
    checkbox->GenericData.Width = fontlib_GetStringWidth(((WIDGET_def*)Template)->Text)
        + fontlib_GetStringWidth(CALC1252_CURSOR_RIGHT " " CALC1252_RADIO_UNCHECKED "  " CALC1252_CURSOR_LEFT);
    return (Widget_t*)checkbox;
}


static int24_t Paint(Widget_t* self)
{
    char ch;
    Style_SetFont(definition->Font);
    fontlib_SetCursorPosition(self->X, self->Y);
    if (this->GenericData.HasFocus)
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
    if (this->GenericData.HasFocus)
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
