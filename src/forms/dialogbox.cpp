#include "dialogbox.h"

using namespace Forms;


Widget_def* DialogBox::GetNextItem(Widget_def* Template)
{
    if (Template == nullptr)
        return nullptr;
    return Container::GetNextItem(&((DialogBox_def*)Template)->Contents);
}


Widget* DialogBox::form_ctor(Widget_def* Template, Widget* parent, Widget_def** Next)
{
    DialogBox* dialogbox = new DialogBox(Template, parent, Next);
    dialogbox->definition = Template;
    dialogbox->parent = parent;
    dialogbox->min_width = ((DialogBox_def*)Template)->MinimumWidth;
    dialogbox->min_height = ((DialogBox_def*)Template)->MinimumHeight;
    // Initialize children
    // Compute size
    Widget** widget = dialogbox->children;
    for (Container_size_t i = dialogbox->count; i > 0; i--)
    {
        dialogbox->height += (*widget)->GetHeight();
    }
    return dialogbox;
}



DialogBox::DialogBox(Widget_def* Template, Widget* Parent, Widget_def** next)
 : Container(&((DialogBox_def*)Template)->Contents, Parent, next)
{
    //
}


extern "C" const Widget_desc DialogBox_desc
{
    ID::WIDGET_ID_DialogBox,
    WIDGET_FLAG_CONTAINER,
    &DialogBox::form_ctor,
    &DialogBox::GetNextItem
};



void DialogBox::Layout()
{
    dirty = true;
    x_t xx = x;
    y_t yy = y;
    Widget** widget = children;
    for (Container_size_t i = count; i > 0; i--)
    {
        (*widget)->MoveTo(xx, yy);
   }
}
