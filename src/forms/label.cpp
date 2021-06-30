#include "label.h"
#include <fontlibc.h>
#include "../style.h"
#include "ignorewarning.h"

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
    widget->font = ((Label_def*)Template)->Font;
    widget->text = ((Label_def*)Template)->Text;
    //reinterpret_cast<Label_def&>(Template).Font;
    Style_SetFont(widget->font);
    widget->height = fontlib_GetCurrentFontHeight();
    widget->width = fontlib_GetStringWidth(widget->text);
    if (next != nullptr)
        *next = (Widget_def*)((Label_def*)Template + 1);
    return widget;
}


extern "C" const Widget_desc Label_desc
{
    ID::Label,
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
    if (hidden)
        return Status::Success;
    Style_SetFont(font);
    fontlib_SetCursorPosition(x, y);
    fontlib_DrawString(text);
    return Status::Success;
}
