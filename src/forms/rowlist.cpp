#include "rowlist.h"

using namespace Forms;


Widget_def* RowList::GetNextItem(Widget_def* Template)
{
    if (Template == nullptr)
        return nullptr;
    return Container::GetNextItem(&((RowList_def*)Template)->Contents);
}


Widget* RowList::form_ctor(Widget_def* Template, Widget* parent, Widget_def** Next)
{
    return new RowList(Template, parent, Next);
}


RowList::RowList(Widget_def* Template, Widget* Parent, Widget_def** next)
 : Container(&((RowList_def*)Template)->Contents, Parent, next)
{
    // Compute size
    unsigned int temp;
    Widget** widget = children;
    for (Container_size_t i = count; i > 0; i--)
    {
        if (width < (temp = (*widget)->GetWidth()))
            width = temp;
        height += (*widget++)->GetHeight();
    }
    min_height = height;
    min_width = width;
}


extern "C" const Widget_desc RowList_desc
{
    ID::WIDGET_ID_RowList,
    WIDGET_FLAG_CONTAINER,
    &RowList::form_ctor,
    &RowList::GetNextItem
};


void RowList::Layout()
{
    dirty = true;
    unsigned int xx = x;
    unsigned char yy = y;
    Widget** child = &children[0];
    for (Container_size_t i = count; i > 0; i--, child++)
    {
        (*child)->MoveTo(xx, yy);
        Container* container = dynamic_cast<Container*>(*child);
        if (container)
            container->Layout();
        yy += (*child)->GetHeight();
    }
}


Status RowList::SetWidth(x_t newwidth)
{
    auto status = Status::Success;
    Widget** child = &children[0];
    for (Container_size_t i = count; i > 0; i--, child++)
        if ((*child)->SetWidth(newwidth) == Status::Failure)
            status = Status::Failure;
    return status;
}
