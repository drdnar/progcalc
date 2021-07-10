#include "rowitems.h"

using namespace Forms;


Widget_def* RowItems::GetNextItem(Widget_def* Template)
{
    if (Template == nullptr)
        return nullptr;
    return Container::GetNextItem(&((RowItems_def*)Template)->Contents);
}


Widget* RowItems::form_ctor(Widget_def* Template, Widget* parent, Widget_def** Next)
{
    return new RowItems(Template, parent, Next);
}


RowItems::RowItems(Widget_def* Template, Widget* Parent, Widget_def** next)
 : Container(&((RowItems_def*)Template)->Contents, Parent, next)
{
    alignment = ((RowItems_def*)Template)->Alignment;
    padding = ((RowItems_def*)Template)->Padding;
    // Initialize children
    // Compute size
    unsigned int widgetHeight;
    Widget** widget = children;
    for (Container_size_t i = count; i > 0; i--, widget++)
    {
        if (height < (widgetHeight = (*widget)->GetHeight()))
            height = widgetHeight;
        width += (*widget)->GetWidth() + padding;
    }
    width -= padding;
    min_width = width;
    min_height = height;
}


extern "C" const Widget_desc RowItems_desc
{
    ID::WIDGET_ID_RowItems,
    WIDGET_FLAG_CONTAINER,
    &RowItems::form_ctor,
    &RowItems::GetNextItem
};





#include <graphx.h>

void RowItems::Layout()
{
    dirty = true;
    x_t xx = x;
    y_t yy = y;
    switch (alignment)
    {
        case HorizontalAlignment::Left:
            // No need to do anything.
            break;
        case HorizontalAlignment::Center:
            xx += (width - min_width) / 2;
            break;
        case HorizontalAlignment::Right:
            xx += width - min_width;
            break;
    }
    Widget** child = children;
    for (Container_size_t i = count; i > 0; i--, child++)
    {
        (*child)->MoveTo(xx, yy);
        Container* container = dynamic_cast<Container*>(*child);
        if (container)
            container->Layout();
        xx += (*child)->GetWidth() + padding;
    }
}


HorizontalAlignment RowItems::GetAlignment()
{
    return alignment;
}


Status RowItems::SetAlignment(HorizontalAlignment Alignment)
{
    alignment = Alignment;
    Layout();
    return Status::Success;
}
