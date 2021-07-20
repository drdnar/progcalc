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
    return new Label(Template, parent, next);
}


extern "C" const Widget_desc Label_desc
{
    ID::WIDGET_ID_Label,
    WIDGET_FLAG_NONE,
    &Label::forms_ctor,
    &Label::GetNextItem
};

Label::Label(Widget_def* Template, Widget* parent, Widget_def** next)
 : Widget(Template, parent, next)
{
    text = ((Label_def*)Template)->Text;
    GetStyle().ActivateFont();
    height = fontlib_GetCurrentFontHeight();
    width = fontlib_GetStringWidth(text);
    disabled = true;
}


Label::~Label()
{
    // Do nothing
    disabled = true;
}


IGNORE_WARNING_UNUSED_PARAMETER
bool Label::SendInput(Message& message)
END_IGNORE_WARNING
{
    return false;
}


void Label::SetText(const char* new_text)
{
    text = new_text;
    GetStyle().ActivateFont();
    width = fontlib_GetStringWidth(text);
    SetDirty();
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
