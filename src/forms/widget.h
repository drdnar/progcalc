/* You should never end up in a situation where you'd include this header twice. */
#include "forms.h"

static Widget_def* GetNextItem(Widget_def* Template);
static Widget_t* ctor(Widget_def* Template, Widget_t* parent);
static void dtor(Widget_t* self);
static uint8_t MoveTo(Widget_t* self, uint24_t X, uint8_t Y);
static int24_t Paint(Widget_t* self);
static int24_t Focus(Widget_t* self);
static int24_t Unfocus(Widget_t* self);
static int24_t Paint(Widget_t* self);
static int24_t SendInput(Widget_t* self, int24_t messageId);
