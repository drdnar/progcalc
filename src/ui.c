#include "ui.h"
#include "style.h"
#include <fontlibc.h>
#include <tice.h>
#include <stdlib.h>
#include <graphx.h>
#include "printbigint.h"
#include "ez80.h"
#include "calc1252.h"
#include "statusbar.h"

Coord_t CursorLocation/* = {0, 0}*/;

#define INDICATOR_HEIGHT 14


/******************************************************************************/

bool Is83Premium(void)
{
    return os_GetSystemInfo()->hardwareType;
}


/******************************************************************************/

sk_key_t GetCSC_APD(void)
{
    sk_key_t key;
    unsigned int timer = RtcTimer_Start(APD_DIM_TIME);
    bool dimmed = false;
    ClearOnKeyPressed();
    do
    {
        if (RtcTimer_Expired(timer))
        {
            if (!dimmed)
            {
                dimmed = true;
                Lcd_Dim();
                timer = RtcTimer_Start(APD_QUIT_TIME);
            }
            else
                ExitClean();
        }
        StatusBar_UpdateBatteryLevel();
        key = GetCSC_Breakable();
    }
    while (!key);
    Lcd_Bright();
    return key;
}


/**
 * Caches the former contents of whatever was under the cursor.
 * This is never freed; the memory just gets recycled every time.
 */
static gfx_sprite_t* formerCursor;

Key_t GetKey(void)
{
    Coord_t oldCursorLoc;
    sk_key_t key;
    bool second = false;
    Style_SaveCursor(&oldCursorLoc);
    gfx_GetSprite(formerCursor, CursorLocation.x, CursorLocation.y);

    do
    {
        key = GetCSC_APD();
        if (key == sk_2nd)
        {
            second = !second;
            if (second)
            {
                Style_RestoreCursor(&CursorLocation);
                Style_SetLargeFontProp();
                fontlib_DrawGlyph(CALC1252_CURSOR_2ND_CHAR);
            }
            else
                gfx_Sprite_NoClip(formerCursor, CursorLocation.x, CursorLocation.y);
        }
        else
            break;
    } while (true);

    if (second)
        key |= sk_2nd_Modifier;
    
    gfx_Sprite_NoClip(formerCursor, CursorLocation.x, CursorLocation.y);
    Style_RestoreCursor(&oldCursorLoc);
    return key;
}


void Ui_Initialize(void)
{
    Style_SetLargeFontProp();
    CursorLocation.x = LCD_WIDTH - fontlib_GetGlyphWidth(CALC1252_CURSOR_2ND_CHAR);
    CursorLocation.y = 0;
    formerCursor = gfx_MallocSprite(fontlib_GetGlyphWidth(CALC1252_CURSOR_2ND_CHAR), INDICATOR_HEIGHT);
    lcd_Palette[COLOR_BATTERY_OUTLINE] = lcd_Palette[COLOR_HIGHLIGHT];
}


void Ui_Finalize(void)
{
    /* I just KNOW that if I don't do this, somehow one day it'll come back to bite me as a memory leak of some kind. */
    free(formerCursor);
}
