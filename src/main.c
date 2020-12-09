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
#include "statusbar.h"

#include "forms/forms.h"
#include "forms/checkbox.h"
#include "forms/label.h"
#include "forms/rowitems.h"
#include "forms/rowlist.h"
#include "testform.h"


/* Main Function */
void main(void) {
    sk_key_t k;
    bool dirty = true;
    
    Style_Initialize();
    Ui_Initialize();
    Settings_Initialize();
    Rpn_Reset();

    /* Need to force stuff to appear in output file. */
    RowList_t* rowlist;
    RowItems_t* rowitems;
    Checkbox_t* checkbox;
    Label_t* label;

    label = (Label_t*)Label_ctor(&TestLabel, NULL, NULL);
    label->Widget.vtable->MoveTo((Widget_t*)label, 10, 10);
    rowitems = (RowItems_t*)RowItems_ctor(&TestRowItems, NULL, NULL);
    rowitems->Widget.vtable->MoveTo((Widget_t*)rowitems, 10, 30);

    do
    {
        if (dirty)
        {
            dirty = false;
            fontlib_ClearWindow();
            Rpn_Redraw();
            StatusBar_Draw();
    label->Widget.vtable->Paint((Widget_t*)label);
    rowitems->Widget.vtable->Paint((Widget_t*)rowitems);
        }
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
                Settings_ChangePrimaryBase(BINARY);
                dirty = true;
                break;
            case sk_Zoom:
                Settings_ChangePrimaryBase(OCTAL);
                dirty = true;
                break;
            case sk_Trace:
                Settings_ChangePrimaryBase(DECIMAL);
                dirty = true;
                break;
            case sk_Graph:
                Settings_ChangePrimaryBase(HEXADECIMAL);
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
