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

/* Main Function */
int main(void) {
    Format_InitDisplaySizes();
    Format_ConfigureDisplaySizes();
    KeyboardEventSource::Enable2nd();
    KeyboardEventSource::EnableIndicator();
    FontManager::SetFont(FONT_LARGE_PROP);
    KeyboardEventSource::SetIndicatorLocation(LCD_WIDTH - fontlib_GetGlyphWidth(CALC1252_CURSOR_2ND_CHAR), 0);

    MessageLoop::EnqueueMessage( 
        { .Id = MESSAGE_GUI_CHANGE_DIALOG, .DataPointer = (void*)&RPN_UI_Dialog }
    );
    MessageLoop::Begin();

    return 0;
}


void ExitClean(void)
{
    exit(0);
}


extern "C" void InitializationError(const char* message)
{
    os_ClrHome();
    os_PutStrFull("Initialization error:");
    os_PutStrFull(message);
    while (!os_GetCSC());
    exit(1);
}
