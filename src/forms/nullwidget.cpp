#include "nullwidget.h"

using namespace Forms;

static Widget* null_ctor(Widget_def* Template, Widget* parent, Widget_def** next)
{
    return nullptr;
}


static Widget_def* null_GetNextItem(Widget_def* Template)
{
    return nullptr;
}


extern "C" const Widget_desc NullWidget_desc
{
    WIDGET_ID_None,
    WIDGET_FLAG_NON_INSTANTIABLE,
    &null_ctor,
    &null_GetNextItem
};
