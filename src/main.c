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

/* Main Function */
void main(void) {
    sk_key_t k;
    bool dirty = false;
    Style_Initialize();
    Settings_Init();
    Settings_ChangeDisplayBits(SHOW_128);
    Rpn_Reset();
    fontlib_ClearWindow();
    Rpn_Redraw();

    do
    {
        do
            k = os_GetCSC();
        while (!k);
        if (GetBigInt_SendKey(k))
        {
            Rpn_SetEntryMode(GetBigInt_IsActive());
            k = 0;
            continue;
        }
        if (Rpn_SendKey(k))
            continue;
        switch (k)
        {
            case sk_Yequ:
                Settings.PrimaryBase = BINARY;
                dirty = true;
                break;
            case sk_Window:
                Settings.PrimaryBase = OCTAL;
                dirty = true;
                break;
            case sk_Zoom:
                Settings.PrimaryBase = DECIMAL;
                dirty = true;
                break;
            case sk_Trace:
                Settings.PrimaryBase = HEXADECIMAL;
                dirty = true;
                break;
            case sk_2nd:
                Settings_ChangeDisplayBits(SHOW_32);
                dirty = true;
                break;
            case sk_Mode:
                Settings_ChangeDisplayBits(SHOW_64);
                dirty = true;
                break;
            case sk_Alpha:
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
    } while (k != sk_Clear);
    
    /* Pause */
    //while (!os_GetCSC());
    Style_Finalize();
}

void ExitClean(void)
{
    Style_Finalize();
    exit(0);
}
