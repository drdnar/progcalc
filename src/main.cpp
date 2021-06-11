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
#include "forms/keyboardeventsource.h"
#include "forms/calc1252.h"
#include "testform.h"

using namespace Forms;

unsigned char someBool = 1;

/* Main Function */
int main(void) {
    sk_key_t k;
    bool dirty = true;

    Settings_Initialize();
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

    StatusBar statusBar;
    statusBar.Show();


    /* Need to force stuff to appear in output file. */
//    RowList_t* rowlist;      
//    RowItems_t* rowitems;
    //Checkbox* checkbox;
    
/*    Label* label = (Label*)TestLabel.TypeDescriptor->ctor(&TestLabel, nullptr, nullptr);
    label->MoveTo(10, 20);
    Checkbox* checkbox = (Checkbox*)TestCheckbox.TypeDescriptor->ctor(&TestCheckbox, nullptr, nullptr);
    checkbox->MoveTo(20, 50);
    //Label* label = (Label*)Label_desc.ctor(&TestLabel, nullptr, nullptr);
    
    RowList* rowlist = (RowList*)TestRowList.TypeDescriptor->ctor(&TestRowList, nullptr, nullptr);
    rowlist->Layout();
    rowlist->MoveTo(15, 100);
    //label = (Label*)Label_ctor(&TestLabel, NULL, NULL);
    //label->Widget.vtable->MoveTo((Widget*)label, 10, 10);
    RowItems* rowitems = (RowItems*)TestRowItems.TypeDescriptor->ctor(&TestRowItems, nullptr, nullptr);
    rowitems->Layout();
    rowitems->MoveTo(15, 125);*/
    
    //rowitems = (RowItems_t*)RowItems_ctor(&TestRowItems, NULL, NULL);
    //rowitems->Widget.vtable->MoveTo((Widget_t*)rowitems, 10, 30);
//    rowlist = (RowList_t*)RowList_ctor(&TestRowList, NULL, NULL);
//    rowlist->Widget.vtable->MoveTo((Widget_t*)rowlist, 10, 30);

    do
    {
        if (dirty)
        {
            dirty = false;
            fontlib_ClearWindow();
            Rpn_Redraw();
            statusBar.Paint();
/*            label->Paint();
            checkbox->Paint();
            rowlist->Paint();
            rowitems->Paint();*/
    //label->Widget.vtable->Paint((Widget*)label);
    //rowitems->Widget.vtable->Paint((Widget_t*)rowitems);
    //rowlist->Widget.vtable->Paint((Widget*)rowlist);
        }
        do
            k = (sk_key_t)keyboard.GetEvent();
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
    return 0;
}


void ExitClean(void)
{
    Settings_Finalize(); /* Run last so Garbage Collect? screen won't cause massive graphical corruption. */
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
