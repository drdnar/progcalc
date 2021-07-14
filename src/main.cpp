#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <tice.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <graphx.h>
#include <fontlibc.h>
#include "bigint.h"
#include "settings.h"
#include "style.h"
#include "printbigint.h"
#include "rpnui.h"
#include "inputbigint.h"
#include "statusbar.h"

#include "forms/widget.h"
#include "forms/checkbox.h"
#include "forms/label.h"
#include "forms/rowitems.h"
#include "forms/rowlist.h"
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
