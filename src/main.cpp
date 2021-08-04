#include <tice.h>
#include <stdlib.h>
#include <fontlibc.h>
#include "printbigint.h"
#include "misc.h"
#include "forms/widget.h"
#include "forms/textmanager.h"
#include "forms/keyboardmessagesource.h"
#include "forms/calc1252.h"
#include "mainform.h"

using namespace Forms;

extern Forms::Widget_def AboutDialog;

/* Main Function */
int main(void) {
    Format_InitDisplaySizes();
    Format_ConfigureDisplaySizes();
    FontManager::SetFont(FONT_LARGE_PROP);
    fontlib_SetFirstPrintableCodePoint(12);
    KeyboardEventSource::SetIndicatorFont(FONT_LARGE_PROP);
    KeyboardEventSource::SetIndicatorChar(CALC1252_CURSOR_2ND_CHAR);
    KeyboardEventSource::SetIndicatorLocation(LCD_WIDTH - fontlib_GetGlyphWidth(CALC1252_CURSOR_2ND_CHAR), 0);
    KeyboardEventSource::Enable2nd();
    KeyboardEventSource::EnableIndicator();
    
    if (Settings::IsFirstRun())
    {
        MessageLoop::EnqueueMessage(
            { .Id = MESSAGE_GUI_CHANGE_DIALOG, .DataPointer = (void*)&AboutDialog }
        );
        MessageLoop::Begin();
    }

    MessageLoop::EnqueueMessage(
        { .Id = MESSAGE_GUI_CHANGE_DIALOG, .DataPointer = (void*)&RPN_UI_Dialog }
    );
    MessageLoop::Begin();

    return 0;
}


extern "C" void InitializationError(const char* message)
{
    os_ClrHome();
    os_PutStrFull("Initialization error:");
    os_PutStrFull(message);
    while (!os_GetCSC());
    exit(1);
}
