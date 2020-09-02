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
#include "ui.h"
#include "printbigint.h"
#include "rpnui.h"

#include "inputbigint.h"

/* Main Function */
void main(void) {
    sk_key_t k;
    bool dirty = false;
    Style_Initialize();
    Ui_Initialize();
    Settings_Initialize();
    Settings_ChangeDisplayBits(SHOW_128);
    Rpn_Reset();
    fontlib_ClearWindow();
    Rpn_Redraw();

    do
    {
        do
            k = GetKey();
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
                Settings.PrimaryBase = BINARY;
                dirty = true;
                break;
            case sk_Zoom:
                Settings.PrimaryBase = OCTAL;
                dirty = true;
                break;
            case sk_Trace:
                Settings.PrimaryBase = DECIMAL;
                dirty = true;
                break;
            case sk_Graph:
                Settings.PrimaryBase = HEXADECIMAL;
                dirty = true;
                break;
            case sk_Alpha:
                Settings_ChangeDisplayBits(SHOW_32);
                dirty = true;
                break;
            case sk_GraphVar:
                Settings_ChangeDisplayBits(SHOW_64);
                dirty = true;
                break;
            case sk_Stat:
                Settings_ChangeDisplayBits(SHOW_128);
                dirty = true;
                break;
        }
        if (dirty)
        {
            dirty = false;
            fontlib_ClearWindow();
            Rpn_Redraw();
        }
    } while (k != sk_Quit);
    
    /* Pause */
    //while (!os_GetCSC());
    ExitClean();
}


void ExitClean(void)
{
    Ui_Finalize();
    Style_Finalize();
    Settings_Finalize(); /* Run last so Garbage Collect? screen won't cause massive graphical corruption. */
    exit(0);
}
