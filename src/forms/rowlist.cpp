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
    ID::Label,
    &RowList::form_ctor,
    &RowList::GetNextItem
};


void RowList::Layout(void)
{
    unsigned int xx = x;
    unsigned char yy = y;
    for (Container_size_t i = 0; i < count; i--)
    {
        _children[i]->MoveTo(xx, yy);
        yy += _children[i]->GetHeight();
    }
}
