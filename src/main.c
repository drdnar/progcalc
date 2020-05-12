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


/* Main Function */
void main(void) {
    uint8_t r;
    
    if (!fontlib_SetFont(fontlib_GetFontByStyle("Courier", 13, 13, 0, 255, 0, 0), 0))
    {
        os_ClrHome();
        os_PutStrFull("Failed to load Courier 13 font.");
        while (!os_GetCSC());
        return;
    }
    
    gfx_Begin();
    /* Erase the screen to black */
    gfx_FillScreen(gfx_black);

    fontlib_DrawString("Nothing happens.");
    fontlib_Newline();
    
    /* Pause */
    while (!os_GetCSC());

    /* Finish the graphics */
    gfx_End();
}
