#include "settingscontroller.h"
#include "forms/button.h"
#include "forms/messages.h"
#include "settings.h"
#include "forms/dialogbox.h"
#include "forms/ignorewarning.h"
#include "forms/enumselector.h"
#include "forms/label.h"

Settings_t TemporarySettings {};


static const char* error_message;

extern "C" void LoadErrorDialogMessage([[maybe_unused]] Forms::DialogBox& sender)
{
    Forms::Container& thingy = dynamic_cast<Forms::Container&>(sender.Get(1));
    Forms::Label& label = dynamic_cast<Forms::Label&>(thingy.Get(0));
    label.SetText(error_message);
}


extern "C" void CloseErrorDialog([[maybe_unused]] Forms::Button& sender)
{
    Forms::MessageLoop::EnqueueMessage({ .Id = Forms::MESSAGE_GUI_MODAL_END, .ExtendedCode = Forms::MESSAGE_NONE });
}


extern "C" bool TrySaveSettings([[maybe_unused]] Forms::Widget& sender)
{
    error_message = Settings::Apply(TemporarySettings);
    if (error_message)
    {
        Forms::MessageLoop::EnqueueMessage( { .Id = Forms::MESSAGE_GUI_MODAL_DIALOG, .DataPointer = &ErrorDialog });
        return false;
    }
    else
        return true;
}


extern "C" void HandleOKCancel(Forms::Button& sender)
{
    if (sender.GetID())
    {
        if (!TrySaveSettings(sender))
            return;
    }
    Forms::MessageLoop::EnqueueMessage({ .Id = Forms::MESSAGE_GUI_MODAL_END, .ExtendedCode = Forms::MESSAGE_NONE });
}


extern "C" void LoadSettings([[maybe_unused]] Forms::DialogBox& sender)
{
    Settings::CopyTo(TemporarySettings);
}
