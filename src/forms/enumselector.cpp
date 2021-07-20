#include "enumselector.h"
#include <fontlibc.h>
#include "textmanager.h"
#include "calc1252.h"
#include "cursorblinker.h"
#include "style.h"

using namespace Forms;

Widget_def* EnumSelector::GetNextItem(Widget_def* Template)
{
    return Widget::GetNextItem(Template, sizeof(EnumSelector_def));
}


Widget* EnumSelector::forms_ctor(Widget_def* Template, Widget* parent, Widget_def** next)
{
    return new EnumSelector(Template, parent, next);
}


EnumSelector::EnumSelector(Widget_def* Template, Widget* parent, Widget_def** next)
    : Widget(Template, parent, next), enum_string { ((EnumSelector_def*)Template)->Enum }
{
GetStyle().ActivateFont();
    text_width = 0;
    const MapTableEntry* entry = enum_string.Values->List;
    for (size_t i = enum_string.Values->Count; i > 0; i--, entry++)
    {
        unsigned int twidth = fontlib_GetStringWidthL((const char*)entry->Out.CPtr, 16);
        if (twidth > text_width)
            text_width = twidth;
    }
    cursor_width = fontlib_GetGlyphWidth(CALC1252_CURSOR_LEFT_CHAR);
    SetWidth(text_width + 2 * cursor_width);
    SetHeight(fontlib_GetCurrentFontHeight());
    variable = ((EnumSelector_def*)definition)->Variable;
    var_size = ((EnumSelector_def*)definition)->VariableSize;
}


extern "C" const Widget_desc EnumSelector_desc
{
    ID::WIDGET_ID_EnumSelector,
    WIDGET_FLAG_NONE,
    &EnumSelector::forms_ctor,
    &EnumSelector::GetNextItem
};


EnumSelector::~EnumSelector()
{
    // Do nothing
}


bool EnumSelector::SendInput(Message& message)
{
//    if (!hasFocus)
//        return false;
    if (message.Id == MESSAGE_BLINK)
        SetDirty();
    else if (message.Id == MESSAGE_KEY)
    {
        const ConstMapTable* table = enum_string.Values;
        unsigned int index = get_index();
        switch (message.ExtendedCode)
        {
            case sk_Left:
                if (index == (unsigned int)-1)
                {
                    enum_string.Write(variable, table->List[0].In);
                    SetDirty();
                    return true;
                }
                if (can_go_lower())
                {
                    enum_string.Write(variable, table->List[index - 1].In);
                    SetDirty();
                    return true;
                }
                else
                    return false;
            case sk_Right:
                if (index == (unsigned int)-1)
                {
                    enum_string.Write(variable, table->List[0].In);
                    SetDirty();
                    return true;
                }
                if (can_go_higher())
                {
                    enum_string.Write(variable, table->List[index + 1].In);
                    SetDirty();
                    return true;
                }
                else
                    return false;
        }
    }
    return false;
}


Status EnumSelector::Paint()
{
    if (!dirty || hidden)
        return Status::Success;
    dirty = false;
    GetStyle().ActivateFont();
    fontlib_SetCursorPosition(x, y);
    if (hasFocus && CursorBlinker::IsShowing() && can_go_lower())
        fontlib_DrawGlyph(CALC1252_CURSOR_LEFT_CHAR);
    else
        fontlib_DrawGlyph(CALC1252_CURSOR_BLANK_CHAR);
    fontlib_DrawString(enum_string.Get(variable));
    if (hasFocus && CursorBlinker::IsShowing() && can_go_higher())
        fontlib_DrawGlyph(CALC1252_CURSOR_RIGHT_CHAR);
    else
        fontlib_DrawGlyph(CALC1252_CURSOR_BLANK_CHAR);
    x_t cx = fontlib_GetCursorX();
    x_t delta = x + width - cx;
    if (delta > 0 && delta < 320)
    {
        gfx_SetColor(GetStyle().GetBackgroundColor());
        gfx_FillRectangle_NoClip(cx, y, delta, height);
    }
    return Status::Success;
}
