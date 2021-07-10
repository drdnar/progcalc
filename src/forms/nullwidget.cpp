#include "nullwidget.h"
#include "ignorewarning.h"

using namespace Forms;

IGNORE_WARNING_UNUSED_PARAMETER
static Widget* null_ctor(Widget_def* Template, Widget* parent, Widget_def** next)
END_IGNORE_WARNING
{
    return nullptr;
}


IGNORE_WARNING_UNUSED_PARAMETER
static Widget_def* null_GetNextItem(Widget_def* Template)
END_IGNORE_WARNING
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
