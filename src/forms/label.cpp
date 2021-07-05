#include "label.h"
#include <fontlibc.h>
#include "textmanager.h"
#include "ignorewarning.h"
#include "style.h"

using namespace Forms;


static Widget_def* GetNextItem(Widget_def* Template)
{
    if (Template == nullptr)
        return nullptr;
    return (Widget_def*)((Label_def*)Template + 1);
}


Widget* Forms::Label_ctor(Widget_def* Template, Widget* parent, Widget_def** next)
{
    Label* widget = new Label();
    widget->definition = Template;
    widget->parent = parent;
    widget->text = ((Label_def*)Template)->Text;
    widget->GetStyle().ActivateFont();
    widget->height = fontlib_GetCurrentFontHeight();
    widget->width = fontlib_GetStringWidth(widget->text);
    if (next != nullptr)
        *next = (Widget_def*)((Label_def*)Template + 1);
    return widget;
}


extern "C" const Widget_desc Label_desc
{
    ID::WIDGET_ID_Label,
    WIDGET_FLAG_NONE,
    &Label_ctor,
    &GetNextItem
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
