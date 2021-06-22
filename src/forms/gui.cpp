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
    form = (Container*)gui->TypeDescriptor->ctor(gui, nullptr, nullptr);
    if (form == nullptr)
        return;
}


GUI::~GUI(void)
{
    delete form;
}


void GUI::GiveControl(void)
{
    do
    {
        /* code */
    } while (true);
    
}
