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


Widget* Style::forms_ctor(Widget_def* Template, Widget* parent, Widget_def** next)
{
    if (next)
    {
        if (Template->TypeDescriptor->TypeId == WIDGET_ID_FullStyleOverride)
            *next = Full_GetNextItem(Template);
        else
            *next = Single_GetNextItem(Template);
    }
    return nullptr;
    // Do stuff here
    // If it's a full override, just iterate over the fields
    // If it's a single override, check subsequent items and apply them, too
}


extern "C" const Widget_desc SingleStyleOverride_desc
{
    ID::WIDGET_ID_SingleStyleOverride,
    WIDGET_FLAG_STYLE_OVERRIDE,
    &Style::forms_ctor,
    &Single_GetNextItem
};


extern "C" const Widget_desc FullStyleOverride_desc
{
    ID::WIDGET_ID_FullStyleOverride,
    WIDGET_FLAG_STYLE_OVERRIDE,
    &Style::forms_ctor,
    &Full_GetNextItem
};


Style::Style(FontId font, Color_t foreground, Color_t background, Color_t highlight, Color_t shadow)
 : parent { nullptr }
{
    bool* b = &overrides[0];
    for (unsigned char i = (int)PropertyID::TotalProperties; i > 0; i--, b++)
        *b = true;
    AnyIntegralType* value = &values[0];
    value++->value8 = font;
    value++->value8 = foreground;
    value++->value8 = background;
    value++->value8 = highlight;
    value++->value8 = shadow;
}


Style::Style(Style* Parent) : parent { Parent }
{
    bool* b = &overrides[0];
    for (unsigned char i = (int)PropertyID::TotalProperties; i > 0; i--, b++)
        *b = false;
}


Style::Style(Style& Parent) : parent { &Parent }
{
    bool* b = &overrides[0];
    for (unsigned char i = (int)PropertyID::TotalProperties; i > 0; i--, b++)
        *b = false;
}


Style::AnyIntegralType Style::get(Style::PropertyID property) const
{
    if (!overrides[(int)property])
        return values[(int)property];
    return parent->get(property);
}


void Style::set(Style::PropertyID property, Style::AnyIntegralType value)
{
    overrides[(int)property] = true;
    values[(int)property] = value;
}


void Style::setDefault(Style::PropertyID property)
{
    if (parent == nullptr)
        return;
    overrides[(int)property] = false;
}


Style* Style::constructify(FullStyleOverride_def* input, Style* parent)
{
    Widget_def* def = (Widget_def*)input;
    Style* style = new Style(parent);
    do
    {
        if (def)
        {
            switch (def->TypeDescriptor->TypeId)
            {
                case WIDGET_ID_SingleStyleOverride:
                {
                    SingleStyleOverride_def* sdef = (SingleStyleOverride_def*)def;
                    style->set(sdef->Property, sdef->Data);
                    break;
                }
                case WIDGET_ID_FullStyleOverride:
                {
                    FullStyleOverride_def* fdef = (FullStyleOverride_def*)def;
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
                    return style;
            }
            def = def->TypeDescriptor->GetNextWidget(def);
        }
        else
            return style;
    } while (true);
}
