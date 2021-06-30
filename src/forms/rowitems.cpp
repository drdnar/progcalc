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
    RowItems* rowitems = new RowItems(Template, Next);
    rowitems->definition = Template;
    rowitems->parent = parent;
    rowitems->_alignment = ((RowItems_def*)Template)->Alignment;
    rowitems->_padding = ((RowItems_def*)Template)->Padding;
    // Initialize children
    // Compute size
    unsigned int height;
    Widget** widget = rowitems->_children;
    for (Container_size_t i = rowitems->count; i > 0; i--)
    {
        if (rowitems->height < (height = (*widget)->GetHeight()))
            rowitems->height = height;
        rowitems->width += (*widget++)->GetWidth() + rowitems->_padding;
    }
    rowitems->width -= rowitems->_padding;
    return rowitems;
}


RowItems::RowItems(Widget_def* Template, Widget_def** next)
 : Container(&((RowItems_def*)Template)->Contents, next)
{
    //
}


extern "C" const Widget_desc RowItems_desc
{
    ID::Label,
    &RowItems::form_ctor,
    &RowItems::GetNextItem
};


void RowItems::Layout()
{
    x_t xx = x;
    y_t yy = y;
    Widget** widget = _children;
    switch (_alignment)
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
    for (Container_size_t i = count; i > 0; i--)
    {
        (*widget)->MoveTo(xx, yy);
        xx += (*widget++)->GetWidth() + _padding;
    }
}


HorizontalAlignment RowItems::GetAlignment()
{
    return _alignment;
}


Status RowItems::SetAlignment(HorizontalAlignment alignment)
{
    _alignment = alignment;
    Layout();
    return Status::Success;
}