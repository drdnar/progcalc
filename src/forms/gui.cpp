#include "gui.h"
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <tice.h>
#include <fontlibc.h>
#include "style.h"

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
    hasFocus = true;
    if (Forms::GUI_DefaultStyle)
    {
        style = Forms::GUI_DefaultStyle;
        style_not_deletable = true;
    }
    else
        style = new Style { 0, 0, 0, 0, 0 };
    height = LCD_HEIGHT;
    width = LCD_WIDTH;
}


Status GUI::Paint()
{
    if (active_dialog)
        return active_dialog->Paint();
    else
        return Status::Failure;
}


bool GUI::SendInput(Message& message)
{
    if (active_dialog)
        return active_dialog->SendInput(message);
    else
        return false;
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
    else if (dialog_count > 1 && message.Id == MESSAGE_GUI_MODAL_END)
    {
        end_dialog();
        return true;
    }
    else
    {
        if (active_dialog)
        {
            if (active_dialog->SendInput(message))
                return true;
            if (message.Id == MESSAGE_GUI_KEY && message.ExtendedCode == GUI_KEY_EXIT)
            {
                if (dialog_count > 1)
                    MessageLoop::EnqueueMessage( { .Id = MESSAGE_GUI_MODAL_END, .ExtendedCode = MESSAGE_NONE } );
                else
                    MessageLoop::EnqueueMessage( { .Id = MESSAGE_EXIT_EVENT_LOOP, .ExtendedCode = MESSAGE_NONE } );
                return true;
            }
        }
        return false;
    }
}


void GUI::begin_dialog(Widget_def* widget)
{
    // This should never happen.
    //if (dialog_count == MAX_DIALOGS)
    //    return;
    if (dialog_count)
        active_dialog->Unfocus();
    active_dialog = (Container*)widget->TypeDescriptor->ctor(widget, &instance, nullptr);
    dialogs[dialog_count++] = active_dialog;
    active_dialog->Layout();
    active_dialog->Focus();
}


void GUI::end_dialog()
{
    if (!dialog_count)
        return;
    delete active_dialog;
    dialogs[--dialog_count] = nullptr;
    if (dialog_count)
    {
        active_dialog = dialogs[dialog_count - 1];
        active_dialog->Focus();
    }
    else
        active_dialog = nullptr;
}


void GUI::flush_dialogs()
{
    while (dialog_count)
        end_dialog();
}


GUI::~GUI()
{
    flush_dialogs();
}


const Style& GUI::GetActiveStyle()
{
    if (instance.active_dialog)
        return instance.active_dialog->GetStyle();
    return instance.GetStyle();
}
