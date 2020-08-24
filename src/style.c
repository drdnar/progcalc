#include "style.h"
#include <fontlibc.h>
#include <tice.h>
#include <stdlib.h>
#include <graphx.h>
#include "printbigint.h"
#include "ez80.h"
#include "calc1252.h"

void ExitClean(void);

Color_t BgColor = 0xFF;
Color_t FgColor = 0x00;
Color_t ZeroColor = 0x56;

Coord_t Format_HexSize;
Coord_t Format_DecSize;
Coord_t Format_OctSize;
Coord_t Format_BinSize;

fontlib_font_t* DrMono14Bold;
fontlib_font_t* DrSans14Bold;
fontlib_font_t* DrSans10Regular;
unsigned int GroupDelimiterWidth;
unsigned char Baseline14;
unsigned char Baseline10;
unsigned char LastFontHeight;

Coord_t CursorLocation/* = {0, 0}*/;

#define INDICATOR_HEIGHT 14

void ShowErrorAndExit(char* msg)
{
    os_ClrHome();
    os_PutStrFull(msg);
    while (!os_GetCSC());
    exit(1);
}


bool Is83Premium(void)
{
    return os_GetSystemInfo()->hardwareType;
}


void Style_PrintCentered(const char* string)
{
    fontlib_SetCursorPosition(fontlib_GetWindowWidth() / 2 + fontlib_GetWindowXMin() - (fontlib_GetStringWidth(string) / 2), fontlib_GetCursorY());
    fontlib_DrawString(string);
}


void Style_PrintRight(const char* string)
{
    fontlib_SetCursorPosition(fontlib_GetWindowWidth() + fontlib_GetWindowXMin() - fontlib_GetStringWidth(string), fontlib_GetCursorY());
    fontlib_DrawString(string);
}


sk_key_t GetCSC_APD(void)
{
    sk_key_t key;
    unsigned int timer = GetRtcSecondsPlus(APD_DIM_TIME);
    bool dimmed = false;
    ClearOnKeyPressed();
    do
    {
        if (GetRtcSeconds() == timer)
        {
            if (!dimmed)
            {
                dimmed = true;
                Lcd_Dim();
                timer = GetRtcSecondsPlus(APD_QUIT_TIME);
            }
            else
                ExitClean();
        }
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
                fontlib_DrawGlyph(CALC1252_CURSOR_2ND_CHAR);
            }
            else
                gfx_Sprite_NoClip(formerCursor, CursorLocation.x, CursorLocation.y);
        }
        else
            break;
    } while (true);

    if (second)
        key |= 0x80;
    
    gfx_Sprite_NoClip(formerCursor, CursorLocation.x, CursorLocation.y);
    Style_RestoreCursor(&oldCursorLoc);
    return key;
}


void Style_Initialize(void)
{
    unsigned char w;
    DrMono14Bold = fontlib_GetFontByStyle("DrMono", 14, 14, FONTLIB_BOLD, FONTLIB_BOLD, 0, 0);
    DrSans10Regular = fontlib_GetFontByStyle("DrSans", 10, 10, FONTLIB_NORMAL, FONTLIB_NORMAL, 0, 0);
    DrSans14Bold = fontlib_GetFontByStyle("DrSans", 14, 14, FONTLIB_BOLD, FONTLIB_BOLD, 0, 0);
    if (!DrSans14Bold || !DrSans10Regular)
        ShowErrorAndExit("Dr. Sans font missing.");
    if (!DrMono14Bold)
        ShowErrorAndExit("Dr. Mono font missing.");
    Baseline14 = DrSans14Bold->baseline_height;
    Baseline10 = DrSans10Regular->baseline_height;
    fontlib_SetFont(DrSans14Bold, 0);
    GroupDelimiterWidth = fontlib_GetGlyphWidth(GROUP_DELIMITER);
    fontlib_SetFont(DrMono14Bold, 0);
    LastFontHeight = 14;
    w = fontlib_GetGlyphWidth(CALC1252_CURSOR_2ND_CHAR);
    CursorLocation.x = LCD_WIDTH - w;
    CursorLocation.y = LCD_HEIGHT - 14;
    formerCursor = gfx_MallocSprite(w, INDICATOR_HEIGHT);
    Format_InitDisplaySizes();
    gfx_Begin();
    gfx_FillScreen(gfx_black);
}


void Style_Finalize(void)
{
    /* Finish the graphics */
    gfx_End();
}


void Style_SaveCursor(Coord_t* target)
{
    target->x = fontlib_GetCursorX();
    target->y = fontlib_GetCursorY();
}


void Style_RestoreCursor(Coord_t* coord)
{
    fontlib_SetCursorPosition(coord->x, coord->y);
}



void Style_SaveTextWindow(CharTextWindow_t* saveTarget)
{
    saveTarget->X = fontlib_GetWindowXMin();
    saveTarget->Y = fontlib_GetWindowYMin();
    saveTarget->Width = fontlib_GetWindowWidth();
    saveTarget->Height = fontlib_GetWindowHeight();
    saveTarget->CursorX = fontlib_GetCursorX();
    saveTarget->CursorY = fontlib_GetCursorY();
}


void Style_RestoreTextWindow(CharTextWindow_t* window)
{
    fontlib_SetWindow(window->X, window->Y, window->Width, window->Height);
    fontlib_SetCursorPosition(window->CursorX, window->CursorY);
}


void Style_SetLargeFontMono(void)
{
    fontlib_SetFont(DrMono14Bold, 0);
    fontlib_SetLineSpacing(0, 0);
    LastFontHeight = 14;
}


void Style_SetLargeFontProp(void)
{
    fontlib_SetFont(DrSans14Bold, 0);
    fontlib_SetLineSpacing(0, 0);
    LastFontHeight = 14;
}


void Style_SetSmallFontProp(void)
{
    fontlib_SetFont(DrSans10Regular, 0);
    fontlib_SetLineSpacing(0, 0);
}


void Style_SetSmallFontPropAligned(void)
{
    unsigned char dh = DrSans14Bold->baseline_height - DrSans10Regular->baseline_height;
    fontlib_SetFont(DrSans10Regular, 0);
    fontlib_SetLineSpacing(dh, DrSans14Bold->height - dh - DrSans10Regular->height);
}