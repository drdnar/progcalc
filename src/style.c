#include "style.h"
#include <fontlibc.h>
#include <tice.h>
#include <stdlib.h>
#include <graphx.h>
#include "printbigint.h"
#include "ez80.h"
#include "calc1252.h"
#include "gfx/gfx.h"

void ExitClean(void);

Coord_t Format_HexSize;
Coord_t Format_DecSize;
Coord_t Format_OctSize;
Coord_t Format_BinSize;

fontlib_font_t* DrMono14Bold;
fontlib_font_t* DrSans14Bold;
fontlib_font_t* DrSans10Regular;
fontlib_font_t* DrSans10Bold;
unsigned int GroupDelimiterWidth;
unsigned char Baseline14;
unsigned char Baseline10;
unsigned char LastFontHeight;


void ShowErrorAndExit(char* msg)
{
    os_ClrHome();
    os_PutStrFull(msg);
    while (!os_GetCSC());
    exit(1);
}


void Style_Initialize(void)
{
    unsigned char w;
    DrMono14Bold = fontlib_GetFontByStyle("DrMono", 14, 14, FONTLIB_BOLD, FONTLIB_BOLD, 0, 0);
    DrSans10Regular = fontlib_GetFontByStyle("DrSans", 10, 10, FONTLIB_NORMAL, FONTLIB_NORMAL, 0, 0);
    DrSans14Bold = fontlib_GetFontByStyle("DrSans", 14, 14, FONTLIB_BOLD, FONTLIB_BOLD, 0, 0);
    DrSans10Bold = fontlib_GetFontByStyle("DrSans", 10, 10, FONTLIB_NORMAL, FONTLIB_BOLD, 0, 0);
    if (!DrSans14Bold || !DrSans10Regular || !DrSans10Bold)
        ShowErrorAndExit("Dr. Sans font missing.");
    if (!DrMono14Bold)
        ShowErrorAndExit("Dr. Mono font missing.");
    Baseline14 = DrSans14Bold->baseline_height;
    Baseline10 = DrSans10Regular->baseline_height;
    fontlib_SetFont(DrSans14Bold, 0);
    GroupDelimiterWidth = fontlib_GetGlyphWidth(GROUP_DELIMITER);
    fontlib_SetFont(DrMono14Bold, 0);
    LastFontHeight = 14;
    CursorLocation.x = LCD_WIDTH - fontlib_GetGlyphWidth(CALC1252_CURSOR_2ND_CHAR);
    CursorLocation.y = 0;
    Format_InitDisplaySizes();
    gfx_Begin();
    gfx_SetPalette(Palette_Global, sizeof_Palette_Global, 0);
    gfx_SetTransparentColor(COLOR_TRANSPARENT);
    gfx_FillScreen(COLOR_ZERO);
}


void Style_Finalize(void)
{
    /* Finish the graphics */
    gfx_End();
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
    LastFontHeight = 14;
}


void Style_SetLargeFontProp(void)
{
    fontlib_SetFont(DrSans14Bold, 0);
    LastFontHeight = 14;
}


void Style_SetSmallFontProp(void)
{
    fontlib_SetFont(DrSans10Regular, 0);
}


void Style_SetSmallFontPropBold(void)
{
    fontlib_SetFont(DrSans10Bold, 0);
}


void Style_SetSmallFontPropAligned(void)
{
    unsigned char dh = DrSans14Bold->baseline_height - DrSans10Regular->baseline_height;
    fontlib_SetFont(DrSans10Regular, 0);
    fontlib_SetLineSpacing(dh, DrSans14Bold->height - dh - DrSans10Regular->height);
}