#include "style.h"

using namespace Forms;


static Widget_def* Single_GetNextItem(Widget_def* Template)
{
    return (Widget_def*)((SingleStyleOverride_def*)Template + 1);
}


static Widget_def* Full_GetNextItem(Widget_def* Template)
{
    return (Widget_def*)((FullStyleOverride_def*)Template + 1);
}


Widget* Style::forms_ctor(Widget_def* Template, [[maybe_unused]] Widget* parent, Widget_def** next)
{
    if (next)
    {
        if (Template->TypeDescriptor->TypeId == WIDGET_ID_FullStyleOverride)
            *next = Full_GetNextItem(Template);
        else
            *next = Single_GetNextItem(Template);
    }
    return nullptr;
}


extern "C" const Widget_desc SingleStyleOverride_desc
{
    ID::WIDGET_ID_SingleStyleOverride,
    WIDGET_FLAG_NON_INSTANTIABLE,
    &Style::forms_ctor,
    &Single_GetNextItem
};


extern "C" const Widget_desc FullStyleOverride_desc
{
    ID::WIDGET_ID_FullStyleOverride,
    WIDGET_FLAG_NON_INSTANTIABLE,
    &Style::forms_ctor,
    &Full_GetNextItem
};


Style::Style(FontId font, Color_t foreground, Color_t background, Color_t highlight, Color_t shadow)
 : parent { nullptr }
{
    bool* b = &overrides[0];
    for (unsigned char i = (unsigned char)PropertyID::TotalProperties; i > 0; i--, b++)
        *b = true;
    AnyIntegralType* value = &values[0];
    *value++ = {font};
    FontManager::EnsureInitialized();
    *value++ = {FontManager::GetFont(font)->baseline_height};
    *value++ = {foreground};
    *value++ = {background};
    *value++ = {highlight};
    *value++ = {shadow};
}


Style::Style(Style* Parent) : parent { Parent }
{
    bool* b = &overrides[0];
    for (unsigned char i = (unsigned char)PropertyID::TotalProperties; i > 0; i--, b++)
        *b = false;
    overrides[0] = true;
    values[0] = {1};
}


Style::Style(Style& Parent) : parent { &Parent }
{
    bool* b = &overrides[0];
    for (unsigned char i = (unsigned char)PropertyID::TotalProperties; i > 0; i--, b++)
        *b = false;
    overrides[0] = true;
    values[0] = {1};
}


Style::AnyIntegralType Style::get(Style::PropertyID property) const
{
    if (overrides[(unsigned char)property])
        return values[(unsigned char)property];
    return parent->get(property);
}


void Style::set(Style::PropertyID property, Style::AnyIntegralType value)
{
    overrides[(unsigned char)property] = true;
    values[(unsigned char)property] = value;
}


void Style::setDefault(Style::PropertyID property)
{
    if (parent == nullptr)
        return;
    overrides[(unsigned char)property] = false;
}


Widget_def* Style::GetNextItem(Widget_def* Template)
{
    do
    {
        if (Template)
        {
            switch (Template->TypeDescriptor->TypeId)
            {
                case WIDGET_ID_SingleStyleOverride:
                    Template = (Widget_def*)((SingleStyleOverride_def*)Template + 1);
                    break;
                case WIDGET_ID_FullStyleOverride:
                    Template = (Widget_def*)((FullStyleOverride_def*)Template + 1);
                    break;
                default:
                    return Template;
            }
        }
        else
            return nullptr;
    } while (true);
}


Style* Style::constructify(Widget_def* input, Style* parent, Widget_def** next)
{
    if (!input)
        return nullptr;
    Style* style = nullptr;
    do
    {
        if (input)
        {
            switch (input->TypeDescriptor->TypeId)
            {
                case WIDGET_ID_SingleStyleOverride:
                {
                    if (!style)
                        style = new Style(parent);
                    SingleStyleOverride_def* sdef = (SingleStyleOverride_def*)input;
                    style->set(sdef->Property, sdef->Data);
                    break;
                }
                case WIDGET_ID_FullStyleOverride:
                {
                    if (!style)
                        style = new Style(parent);
                    FullStyleOverride_def* fdef = (FullStyleOverride_def*)input;
                    // WARNING: This code is fragile.
                    uint8_t* property = (uint8_t*)&fdef->OverrideFont;
                    for (uint8_t i = 0; i < (uint8_t)PropertyID::TotalProperties; i++)
                    {
                        if (*property++)
                            style->set((PropertyID)i, {*property++} );
                        else
                            property++;
                    }
                    break;
                }
                default:
                    if (next)
                        *next = input;
                    return style;
            }
            input = input->TypeDescriptor->GetNextWidget(input);
        }
        else
        {
            if (next)
                *next = input;
            return style;
        }
    } while (true);
    return nullptr;
}


void Style::SetFont(FontId font)
{
    set(PropertyID::Font, {font});
    set(PropertyID::FontBaseline, {FontManager::GetFont(font)->baseline_height});
}
