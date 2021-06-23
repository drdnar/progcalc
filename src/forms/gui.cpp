#include "gui.h"
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <tice.h>
#include <fontlibc.h>
#include <graphx.h>

using namespace Forms;


GUI GUI::instance;

Container* GUI::dialogs[MAX_DIALOGS]
{
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
};

unsigned char GUI::dialog_count { 0 };

Container* GUI::active_dialog { nullptr };


GUI::GUI() : MessageSink(SINK_PRIORITY_NORMAL)
{
    MessageLoop::RegisterMessageSink(*this);
}


bool GUI::SendMessage(Message& message)
{
    if (message.Id == MESSAGE_GUI_CHANGE_DIALOG || message.Id == MESSAGE_GUI_MODAL_DIALOG)
    {
        if (dialog_count == MAX_DIALOGS)
            return false;
        if (message.Id == MESSAGE_GUI_CHANGE_DIALOG)
            flush_dialogs();
        begin_dialog((Widget_def*)message.DataPointer);
        return true;
    }
    else
    {
        return active_dialog->SendInput(message);
    }
}


void GUI::begin_dialog(Widget_def* widget)
{
    active_dialog = (Container*)widget->TypeDescriptor->ctor(widget, nullptr, nullptr);
    dialogs[dialog_count++] = active_dialog;
    active_dialog->Layout();
}


void GUI::end_dialog(void)
{
    if (dialog_count == 0)
        return;
    delete active_dialog;
    dialogs[--dialog_count] = nullptr;
    if (dialog_count > 0)
        active_dialog = dialogs[dialog_count - 1];
    else
        active_dialog = nullptr;
}


void GUI::flush_dialogs(void)
{
    Container** item = &dialogs[0];
    for (unsigned char i = MAX_DIALOGS; i > 0; i--)
        delete item++;
    dialog_count = 0;
}


GUI::~GUI(void)
{
    flush_dialogs();
}

