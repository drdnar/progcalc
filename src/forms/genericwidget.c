#include "forms.h"
#include <string.h>

Widget_vtable* Clone_Widget_vtable(const Widget_vtable* src)
{
    Widget_vtable* vtable = malloc(src->Length);
    memcpy(vtable, src, src->Length);
    vtable->IsDynamic = true;
    return vtable;
}


void GenericWidget_dtor(Widget_t* self)
{
    if (((Widget_t*)self)->vtable->IsDynamic)
        free(((Widget_t*)self)->vtable);
    free(self);
}


int24_t GenericWidget_SuccessEvent(Widget_t* self)
{
    return 0;
}


int24_t GenericWidget_FailureEvent(Widget_t* self)
{
    return 1;
}


int24_t GenericWidget_Focus(Widget_t* self)
{
    self->HasFocus = true;
    self->vtable->Paint(self);
    return 0;
}


int24_t GenericWidget_Unfocus(Widget_t* self)
{
    self->HasFocus = false;
    self->vtable->Paint(self);
    return 0;
}


uint8_t GenericWidget_MoveTo(Widget_t* self, uint24_t x, uint8_t y)
{
    self->X = x;
    self->Y = y;
    return 0;
}


int24_t GenericWidget_SendInput(Widget_t* self, int24_t messageId)
{
    return 0;
}
