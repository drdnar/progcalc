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
#include "testform.h"

using namespace Forms;

unsigned char someBool = 1;

/* Main Function */
int main(void) {
    sk_key_t k;
    bool dirty = true;

    Message message;

    Format_InitDisplaySizes();
    // These two are also called by Settings_ChangeDisplayBits
    Format_ConfigureDisplaySizes();
    GetBigInt_Reposition();
    Rpn_Reset();
    
    KeyboardEventSource& keyboard = KeyboardEventSource::GetInstance();
    KeyboardEventSource::Enable2nd();
    KeyboardEventSource::EnableIndicator();
    FontManager::SetFont(FONT_LARGE_PROP);
    KeyboardEventSource::SetIndicatorLocation(LCD_WIDTH - fontlib_GetGlyphWidth(CALC1252_CURSOR_2ND_CHAR), 0);

    StatusBar& statusBar = StatusBar::GetInstance();
    statusBar.Show();

    do
    {
        if (dirty)
        {
            dirty = false;
            fontlib_ClearWindow();
            Rpn_Redraw();
            statusBar.Paint();
        }
        do
        {
            message = keyboard.GetMessage();
            if (message.Id == MESSAGE_NONE)
                continue;
            k = (sk_key_t)message.ExtendedCode;
        }
        while (!k);
        if (GetBigInt_SendKey(k))
        {
            Rpn_SetInputMode(GetBigInt_IsActive());
            continue;
        }
        if (Rpn_SendKey(k))
            continue;
        switch (k)
        {
            case sk_Window:
                Settings::SetPrimaryBase(BINARY);
                dirty = true;
                break;
            case sk_Zoom:
                Settings::SetPrimaryBase(OCTAL);
                dirty = true;
                break;
            case sk_Trace:
                Settings::SetPrimaryBase(DECIMAL);
                dirty = true;
                break;
            case sk_Graph:
                Settings::SetPrimaryBase(HEXADECIMAL);
                dirty = true;
                break;
            case sk_Alpha:
                Settings::SetDisplayBits(SHOW_32);
                dirty = true;
                break;
            case sk_GraphVar:
                Settings::SetDisplayBits(SHOW_64);
                dirty = true;
                break;
            case sk_Stat:
                Settings::SetDisplayBits(SHOW_128);
                dirty = true;
                break;
        }
    } while (k != sk_Quit);
    
    /* Pause */
    //while (!os_GetCSC());
    ExitClean();
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
