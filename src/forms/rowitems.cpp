#include "rowitems.h"

using namespace Forms;


static Widget_def* GetNextItem(Widget_def* Template)
{
    if (Template == nullptr)
        return nullptr;
    return Container::GetNextItem(&((RowItems_def*)Template)->Contents);
}


Widget* Forms::RowItems_ctor(Widget_def* Template, Widget* parent, Widget_def** Next)
{
    RowItems* rowitems = new RowItems();
    rowitems->_definition = Template;
    rowitems->_parent = parent;
    rowitems->_alignment = ((RowItems_def*)Template)->Alignment;
    // Initialize children
    Container_ctor(&((RowItems_def*)Template)->Contents, *rowitems, Next);
    // Compute size
    unsigned int height;
    Widget** widget = rowitems->_children;
    for (Container_size_t i = rowitems->_count; i > 0; i--)
    {
        if (rowitems->_height < (height = (*widget)->GetHeight()))
            rowitems->_height = height;
        rowitems->_width += (*widget++)->GetWidth();
    }
    return rowitems;
}


extern "C" const Widget_desc RowItems_desc
{
    ID::Label,
    &RowItems_ctor,
    &GetNextItem
};


void RowItems::Layout(void)
{
    unsigned int x = _x;
    unsigned char y = _y;
    Widget** widget = _children;
    switch (_alignment)
    {
        case HorizontalAlignment::Left:
            // No need to do anything.
            break;
        case HorizontalAlignment::Center:
            x += (_width - _min_width) / 2;
            break;
        case HorizontalAlignment::Right:
            x += _width - _min_width;
            break;
    }
    for (Container_size_t i = _count; i > 0; i--)
    {
        (*widget)->MoveTo(x, y);
        x += (*widget++)->GetWidth();
    }
}


HorizontalAlignment RowItems::GetAlignment(void)
{
    return _alignment;
}


Status RowItems::SetAlignment(HorizontalAlignment alignment)
{
    _alignment = alignment;
    Layout();
    return Status::Success;
}
