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
    RowList* rowlist = new RowList(Template, Next);
    rowlist->definition = Template;
    rowlist->parent = parent;
    // Compute size
    unsigned int width = 0, temp;
    unsigned char height = 0;
    Widget** widget = rowlist->_children;
    for (Container_size_t i = rowlist->count; i > 0; i--)
    {
        if (width < (temp = (*widget)->GetWidth()))
            width = temp;
        height += (*widget++)->GetHeight();
    }
    rowlist->height = rowlist->min_height = height;
    rowlist->width = rowlist->min_width = width;
    return rowlist;
}


RowList::RowList(Widget_def* Template, Widget_def** next)
 : Container(&((RowList_def*)Template)->Contents, next)
{
    //
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
    Widget** child = &_children[0];
    for (Container_size_t i = count; i > 0; i--, child++)
    {
        (*child)->MoveTo(xx, yy);
        yy += (*child)->GetHeight();
    }
}
