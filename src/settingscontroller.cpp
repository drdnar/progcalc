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

extern "C" void SettingsController_LoadErrorDialogMessage([[maybe_unused]] Forms::DialogBox& sender)
{
    Forms::Container& body = dynamic_cast<Forms::Container&>(sender.Get(1));
    Forms::Label& label = dynamic_cast<Forms::Label&>(body.Get(0));
    label.SetHeight(body.GetHeight());
    label.SetText(error_message);
}


extern "C" bool SettingsController_HandleGuiEvent([[maybe_unused]] Forms::Widget& sender, Forms::MessageCode id)
{
    if (id != Forms::GUI_EVENT_OK)
        return false;
    if ((error_message = Settings::Apply(TemporarySettings)) == nullptr)
        return false;
    Forms::MessageLoop::EnqueueMessage({ .Id = Forms::MESSAGE_GUI_MODAL_DIALOG, .DataPointer = &ErrorDialog});
    return true;
}


extern "C" void SettingsController_HandleOKCancel(Forms::Button& sender)
{
    Forms::MessageCode code;
    if (sender.GetID())
        code = Forms::GUI_EVENT_OK;
    else
        code = Forms::GUI_EVENT_CANCEL;
    Forms::MessageLoop::EnqueueMessage({ .Id = Forms::MESSAGE_GUI_EVENT, .ExtendedCode = code });
}


extern "C" void SettingsController_LoadSettings([[maybe_unused]] Forms::DialogBox& sender)
{
    Settings::CopyTo(TemporarySettings);
}
