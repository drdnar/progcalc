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

IGNORE_WARNING_UNUSED_PARAMETER
extern "C" void LoadErrorDialogMessage(Forms::DialogBox& sender)
END_IGNORE_WARNING
{
    Forms::Container& thingy = dynamic_cast<Forms::Container&>(sender.Get(1));
    Forms::Label& label = dynamic_cast<Forms::Label&>(thingy.Get(0));
    label.SetText(error_message);
}


IGNORE_WARNING_UNUSED_PARAMETER
extern "C" void CloseErrorDialog(Forms::Button& sender)
END_IGNORE_WARNING
{
    Forms::MessageLoop::EnqueueMessage({ .Id = Forms::MESSAGE_GUI_MODAL_END, .ExtendedCode = Forms::MESSAGE_NONE });
}


IGNORE_WARNING_UNUSED_PARAMETER
extern "C" bool TrySaveSettings(Forms::Widget& sender)
END_IGNORE_WARNING
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


IGNORE_WARNING_UNUSED_PARAMETER
extern "C" void LoadSettings(Forms::DialogBox& sender)
END_IGNORE_WARNING
{
    Settings::CopyTo(TemporarySettings);
}
