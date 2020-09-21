#define WIDGET Label
#include "widget.h"
#include "label.h"
#include <fontlibc.h>

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
    &GenericWidget_FailureEvent,
    /* Unfocus */
    &GenericWidget_SuccessEvent,
    &GenericWidget_SendInput
};


static Widget_def* GetNextItem(Widget_def* Template)
{
    return (Widget_def*)((Label_t*)Template + 1);
}


static Widget_t* ctor(Widget_def* Template, Widget_t* parent)
{
    Label_t* label = (WIDGET_t*)malloc(sizeof(WIDGET_t));
    label->GenericData.TypeId = LABEL;
    label->GenericData.vtable = &vtable;
    label->GenericData.Definition = Template;
    label->GenericData.Parent = parent;
    Style_SetFont(((WIDGET_def*)Template)->Font);
    label->GenericData.Height = fontlib_GetCurrentFontHeight();
    label->GenericData.Width = fontlib_GetStringWidth(((WIDGET_def*)Template)->Text);
    return (Widget_t*)label;
}


static int24_t Paint(Widget_t* self)
{
    Style_SetFont(definition->Font);
    fontlib_SetCursorPosition(self->X, self->Y);
    fontlib_DrawString(definition->Text);
    return 0;
}
