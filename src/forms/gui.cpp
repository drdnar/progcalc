#include "gui.h"
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <tice.h>
#include <fontlibc.h>
#include <graphx.h>

using namespace Forms;


GUI::GUI(Widget_def* gui)
{
    _form = (Container*)gui->TypeDescriptor->ctor(gui, nullptr, nullptr);
    if (_form == nullptr)
        return;
}


GUI::~GUI(void)
{
    delete _form;
}


void GUI::GiveControl(void)
{
    do
    {
        /* code */
    } while (true);
    
}
