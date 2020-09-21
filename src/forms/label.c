#include "label.h"
#include <fontlibc.h>

#define this ((Label_t*)self)
#define thisData ((Label_def*)this->GenericData.Definition)


static const Widget_vtable vtable =
{
    &GetNextItem,
    &ctor,
    &Dummy_dtor,
    &Dummy_MoveTo,
    /* Paint */
    &Paint,
    /* Focus */
    &DummyFailure_Event,
    /* Unfocus */
    &DummySuccess_Event,
    &Dummy_SendInput
};


static Widget_def* GetNextItem(Widget_def* Template)
{
    return (Widget_def*)((Label_t*)Template + 1);
}


static Widget_t* ctor(Widget_def* Template, Widget_t* parent)
{
    Label_t* label = (Label_t*)malloc(sizeof(Label_t));
    label->GenericData.TypeId = LABEL;
    label->GenericData.vtable = &vtable;
    label->GenericData.Definition = Template;
    label->GenericData.Parent = parent;
    Style_SetFont(((Label_def*)Template)->Font);
    label->GenericData.Height = fontlib_GetCurrentFontHeight();
    label->GenericData.Width = fontlib_GetStringWidth(((Label_def*)Template)->Text);
    return (Widget_t*)label;
}


static int24_t Paint(Widget_t* self)
{
    Style_SetFont(thisData->Font);
    fontlib_SetCursorPosition(self->X, self->Y);
    fontlib_DrawString(thisData->Text);
    return 0;
}
