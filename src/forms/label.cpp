#define WIDGET Label
#define TYPEID LABEL
#include "widget.inc.h"
#include "label.h"
#include <fontlibc.h>
#include "../style.h"

const WIDGET_vtable_t WIDGET_vtable =
{
    /* Widget */
    {
        sizeof(WIDGET_vtable_t),
        false,
        &GetNextItem,
        &WIDGET_ctor,
        &GenericWidget_dtor,
        &GenericWidget_MoveTo,
        &Paint,
        /* Focus */
        &GenericWidget_FailureEvent,
        /* Unfocus */
        &GenericWidget_SuccessEvent,
        &GenericWidget_SendInput
    }
};


static const Widget_def* GetNextItem(const Widget_def* Template)
{
    return (Widget_def*)((WIDGET_t*)Template + 1);
}


Widget_t* WIDGET_ctor(const Widget_def* Template, Widget_t* parent, Widget_def** next)
{
    WIDGET_t* widget = (WIDGET_t*)malloc(sizeof(WIDGET_t));
    widget->Widget.TypeId = TYPEID;
    widget->Widget.vtable = (Widget_vtable*)&WIDGET_vtable;
    widget->Widget.Definition = Template;
    widget->Widget.Parent = parent;
    Style_SetFont(((WIDGET_def*)Template)->Font);
    widget->Widget.Height = fontlib_GetCurrentFontHeight();
    widget->Widget.Width = fontlib_GetStringWidth(((WIDGET_def*)Template)->Text);
    if (next != NULL)
        *next = (Widget_def*)((WIDGET_def*)Template + 1);
    return (Widget_t*)widget;
}


static int24_t Paint(Widget_t* self)
{
    Style_SetFont(definition->Font);
    fontlib_SetCursorPosition(self->X, self->Y);
    fontlib_DrawString(definition->Text);
    return 0;
}
