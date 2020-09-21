#include "forms.h"

void Dummy_dtor(Widget_t* self)
{
    free(self);
}


int24_t DummySuccess_Event(Widget_t* self)
{
    return 0;
}


int24_t DummyFailure_Event(Widget_t* self)
{
    return 1;
}


int24_t Dummy_Focus(Widget_t* self)
{
    self->HasFocus = true;
    self->vtable->Paint(self);
}


int24_t Dummy_Unfocus(Widget_t* self)
{
    self->HasFocus = false;
    self->vtable->Paint(self);
}


uint8_t Dummy_MoveTo(Widget_t* self, uint24_t x, uint8_t y)
{
    self->X = x;
    self->Y = y;
    return 0;
}


int24_t Dummy_SendInput(Widget_t* self, int24_t messageId)
{
    return 0;
}
