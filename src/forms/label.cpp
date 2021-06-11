#include "label.h"
#include <fontlibc.h>
#include "../style.h"

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
    widget->_definition = Template;
    widget->_parent = parent;
    widget->_font = ((Label_def*)Template)->Font;
    widget->_text = ((Label_def*)Template)->Text;
    //reinterpret_cast<Label_def&>(Template).Font;
    Style_SetFont(widget->_font);
    widget->_height = fontlib_GetCurrentFontHeight();
    widget->_width = fontlib_GetStringWidth(widget->_text);
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


Status Label::Paint()
{
    if (_hidden)
        return Status::Success;
    Style_SetFont(_font);
    fontlib_SetCursorPosition(_x, _y);
    fontlib_DrawString(_text);
    return Status::Success;
}
