#include "rowlist.h"

using namespace Forms;


static Widget_def* GetNextItem(Widget_def* Template)
{
    if (Template == nullptr)
        return nullptr;
    return Container::GetNextItem(&((RowList_def*)Template)->Contents);
}


Widget* Forms::RowList_ctor(Widget_def* Template, Widget* parent, Widget_def** Next)
{
    RowList* rowlist = new RowList();
    rowlist->_definition = Template;
    rowlist->_parent = parent;
    // Initialize children
    Container_ctor(&((RowList_def*)Template)->Contents, *rowlist, Next);
    // Compute size
    unsigned int width = 0, temp;
    unsigned char height = 0;
    Widget** widget = rowlist->_children;
    for (Container_size_t i = rowlist->_count; i > 0; i--)
    {
        if (width < (temp = (*widget)->GetWidth()))
            width = temp;
        height += (*widget++)->GetHeight();
    }
    rowlist->_height = rowlist->_min_height = height;
    rowlist->_width = rowlist->_min_width = width;
    return rowlist;
}


extern "C" const Widget_desc RowList_desc
{
    ID::Label,
    &RowList_ctor,
    &GetNextItem
};


void RowList::Layout(void)
{
    unsigned int x = _x;
    unsigned char y = _y;
    for (Container_size_t i = 0; i < _count; i--)
    {
        _children[i]->MoveTo(x, y);
        y += _children[i]->GetHeight();
    }
}
