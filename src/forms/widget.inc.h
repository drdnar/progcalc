/**
 *  You should never end up in a situation where you'd include this header twice.
 */
#define WIDGET_H
#include "forms.h"

#define EXPAND_ARGUMENT(arg) arg
/**
 * Name of this widget's changing data struct.
 * Allows easy copy-and-paste of code.
 */
#define WIDGET_t EXPAND_ARGUMENT(WIDGET) ## _t
/**
 * Pointer to this widget's changable data, such as position.
 */
#define this ((WIDGET_t*)self)
/**
 * Name of this widget's definition struct.
 * Allows easy copy-and-paste of code.
 */
#define WIDGET_def EXPAND_ARGUMENT(WIDGET) ## _def
/**
 * Pointer to this widget's unchanging data, such as text it displays.
 */
#define definition ((WIDGET_def*)this->Widget.Definition)
/**
 * The Template variable, but typecast to this widget's type.
 */
#define template ((WIDGET_def*)Template)
/**
 * Type name of this widget's expanded vtabel, if applicable.
 */
#define WIDGET_vtable EXPAND_ARGUMENT(WIDGET) ## _vtable
/**
 * Name of this widget's public constructor.
 */
#define WIDGET_ctor EXPAND_ARGUMENT(WIDGET) ## _ctor

static Widget_def* GetNextItem(Widget_def* Template);
static Widget_t* WIDGET_ctor(const Widget_def* Template, Widget_t* parent, Widget_def** next);
static void dtor(Widget_t* self);
static uint8_t MoveTo(Widget_t* self, uint24_t X, uint8_t Y);
static int24_t Paint(Widget_t* self);
static int24_t Focus(Widget_t* self);
static int24_t Unfocus(Widget_t* self);
static int24_t SendInput(Widget_t* self, int24_t messageId);
