#include "settingscontroller.h"
#include "forms/button.h"
#include "forms/messages.h"
#include "settings.h"
#include "forms/dialogbox.h"
#include "forms/ignorewarning.h"

Settings_t TemporarySettings {};


extern "C" void HandleOKCancel(Forms::Button& sender)
{
    if (sender.GetID() == 1)
    {
        if (TemporarySettings.StatusBarEnabled != Settings::GetStatusBar())
            Settings::SetStatusBar(TemporarySettings.StatusBarEnabled);
    }
    Forms::MessageLoop::EnqueueMessage({ .Id = Forms::MESSAGE_GUI_MODAL_END, .ExtendedCode = Forms::MESSAGE_NONE });
}


IGNORE_WARNING_UNUSED_PARAMETER
extern "C" void LoadSettings(Forms::DialogBox& sender)
END_IGNORE_WARNING
{
    Settings::CopyTo(TemporarySettings);
}
