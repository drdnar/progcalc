#include "label.h"
#include <fontlibc.h>
#include "textmanager.h"
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
    WindowSaver saver;
    text = ((Label_def*)Template)->Text;
    GetStyle().ActivateFont();
    disabled = true;
    Coord size;
    WordWrap::GetTextDimensions(text, size, LCD_WIDTH);
    width = size.x;
    height = size.y;
}


void Label::SetText(const char* new_text)
{
    text = new_text;
    SetDirty();
}


Status Label::Paint()
{
    if (!dirty || hidden)
        return Status::Success;
    dirty = false;
    WindowSaver saver;
    setWindow();
    fontlib_HomeUp();
    GetStyle().ActivateFont();
    WordWrap::Print(text);
    return Status::Success;
}
