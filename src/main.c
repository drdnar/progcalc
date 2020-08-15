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

/* 0x12345678901234567890123456789012 */
BigInt_t n1 = { 0x12, 0x90, 0x78, 0x56, 0x34, 0x12, 0x90, 0x78, 0x56, 0x34, 0x12, 0x90, 0x78, 0x56, 0x34, 0x12, };
/* 0x7890123456789012 */
BigInt_t n2 = { 0x12, 0x90, 0x78, 0x56, 0x34, 0x12, 0x90, 0x78, 0, 0, 0, 0, 0, 0, 0, 0, };


/* Main Function */
void main(void) {
    sk_key_t k;
    bool dirty = false;
    Style_Initialize();
    Settings_Init();
    
    Settings_ChangeDisplayBits(SHOW_128);

    /*fontlib_DrawString("TESTING");
    fontlib_Newline();
    Format_PrintBin(&n1);
    Format_PrintHex(&n1);
    Format_PrintDec(&n2);*/
    
    Rpn_Reset();
    /*fontlib_Newline();
    Style_SetSmallFontProp();
    fontlib_DrawString("bin"); fontlib_Newline();
    fontlib_DrawString("oct"); fontlib_Newline();
    fontlib_DrawString("dec"); fontlib_Newline();
    Style_SetLargeFontProp(); fontlib_DrawString("00:");
    Style_SetSmallFontPropAligned(); fontlib_DrawString(" hex "); //fontlib_Newline();
    */
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
            continue;
        }
        if (Rpn_SendKey(k))
            continue;
        switch (k)
        {
            case sk_Del:
                /* Delete top entry */
                break;
            case sk_Enter:
                /* Duplicate top entry */
                break;
            case sk_Clear:
                if (!GetBigInt_IsActive())
                    break;
                GetBigInt_Reset();
                k = 0;
                dirty = true;
                break;
            case sk_Yequ:
                Settings.PrimaryBase = BINARY;
                dirty = true;
                break;
            /*case sk_Window:
                Settings.PrimaryBase = OCTAL;
                dirty = true;
                break;*/
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
