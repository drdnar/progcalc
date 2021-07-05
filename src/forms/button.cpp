#include "button.h"
#include <fontlibc.h>
#include "textmanager.h"
#include "ignorewarning.h"
#include "ez80.h"

using namespace Forms;


Widget_def* Button::GetNextItem(Widget_def* Template)
{
    if (Template == nullptr)
        return nullptr;
    return (Widget_def*)((Button_def*)Template + 1);
}


Widget* Button::forms_ctor(Widget_def* Template, Widget* parent, Widget_def** next)
{
    Button* widget = new Button();
    widget->definition = Template;
    widget->parent = parent;
    //widget->font = ((Button_def*)Template)->Font;
    //widget->text = ((Button_def*)Template)->Text;
    widget->min_width = ((Button_def*)Template)->MinimumWidth;
    //FontManager::SetFont(widget->font);
    widget->ActivateFont();
    widget->height = fontlib_GetCurrentFontHeight() + 4;
    widget->textWidth = fontlib_GetStringWidth(widget->GetText());
    widget->width = widget->textWidth + 4;
    if (widget->width < widget->min_width)
        widget->width = widget->min_width;
    if (next != nullptr)
        *next = (Widget_def*)((Button_def*)Template + 1);
    return widget;
}


extern "C" const Widget_desc Button_desc
{
    ID::WIDGET_ID_Button,
    WIDGET_FLAG_NONE,
    &Button::forms_ctor,
    &Button::GetNextItem
};


Button::~Button()
{
    // Do nothing
}


bool Button::SendInput(Message& message)
{
    if (message.Id != MESSAGE_KEY)
        return false;
    if (message.ExtendedCode == sk_Enter)
    {
        Press();
        return true;
    }
    return false;
}


Status Button::Paint()
{
    if (!dirty || hidden)
        return Status::Success;
    dirty = false;
    // Draw box
    uint8_t fg = fontlib_GetForegroundColor();
    uint8_t bg = fontlib_GetBackgroundColor();
    if (!disabled)
        gfx_SetColor(fg);
    else
        gfx_SetColor(bg);
    gfx_Rectangle_NoClip(x, y, width, height);
    gfx_SetColor(bg);
    gfx_SetPixel(x, y);
    gfx_SetPixel(x + width, y);
    gfx_SetPixel(x, y + height);
    gfx_SetPixel(x + width, y + height);
    gfx_Rectangle_NoClip(x + 1, y + 1, width - 2, height - 2);
    // Erase whitespace on left of text
    unsigned int leftpad = (width - 4) / 2 - textWidth / 2;
    gfx_FillRectangle_NoClip(x + 2, y + 2, leftpad, height - 4);
    ActivateFont();
    fontlib_SetCursorPosition(x + 2 + leftpad, y + 2);
    fontlib_DrawString(GetText());
    // Erase whitespace on right of text
    unsigned int xx = fontlib_GetCursorX();
    gfx_FillRectangle_NoClip(xx, y + 2, x + width - 2 - xx, height - 4);
    return Status::Success;
}


void Button::Press()
{
    auto callback = ((Button_def*)definition)->OnPress;
    if (!callback)
        callback(*this);
}

