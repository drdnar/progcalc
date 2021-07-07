#include "label.h"
#include <fontlibc.h>
#include "textmanager.h"
#include "ignorewarning.h"
#include "style.h"

using namespace Forms;


Widget_def* Label::GetNextItem(Widget_def* Template)
{
    return Widget::GetNextItem(Template, sizeof(Label_def));
}


Widget* Label::forms_ctor(Widget_def* Template, Widget* parent, Widget_def** next)
{
    Label* widget = new Label(Template, parent, next);
    widget->text = ((Label_def*)widget->definition)->Text;
    widget->GetStyle().ActivateFont();
    widget->height = fontlib_GetCurrentFontHeight();
    widget->width = fontlib_GetStringWidth(widget->text);
    return widget;
}


extern "C" const Widget_desc Label_desc
{
    ID::WIDGET_ID_Label,
    WIDGET_FLAG_NONE,
    &Label::forms_ctor,
    &Label::GetNextItem
};


Label::~Label()
{
    // Do nothing
}


IGNORE_WARNING_UNUSED_PARAMETER
bool Label::SendInput(Message& message)
END_IGNORE_WARNING
{
    return false;
}


Status Label::Paint()
{
    if (!dirty || hidden)
        return Status::Success;
    dirty = false;
    GetStyle().ActivateFont();
    fontlib_SetCursorPosition(x, y);
    fontlib_DrawString(text);
    return Status::Success;
}
