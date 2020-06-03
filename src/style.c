#include "style.h"
#include <fontlibc.h>
#include <tice.h>
#include <stdlib.h>


Color_t BgColor = 0xFF;
Color_t FgColor = 0x00;
Color_t ZeroColor = 0x56;

CharCoord_t Format_HexSize;
CharCoord_t Format_DecSize;
CharCoord_t Format_OctSize;
CharCoord_t Format_BinSize;

fontlib_font_t* DrMono14Bold;
fontlib_font_t* DrSans14Bold;
fontlib_font_t* DrSans9Regular;


void ShowErrorAndExit(char* msg)
{
    os_ClrHome();
    os_PutStrFull(msg);
    while (!os_GetCSC());
    exit(1);
}


void Style_Initialize(void)
{
    DrMono14Bold = fontlib_GetFontByStyle("DrMono", 14, 14, FONTLIB_BOLD, FONTLIB_BOLD, 0, 0);
    DrSans9Regular = fontlib_GetFontByStyle("DrSans", 9, 9, FONTLIB_NORMAL, FONTLIB_NORMAL, 0, 0);
    DrSans14Bold = fontlib_GetFontByStyle("DrSans", 14, 14, FONTLIB_BOLD, FONTLIB_BOLD, 0, 0);
    if (!DrSans14Bold || !DrSans9Regular)
        ShowErrorAndExit("Dr. Sans font missing.");
    if (!DrMono14Bold)
        ShowErrorAndExit("Dr. Mono font missing.");
    fontlib_SetFont(DrMono14Bold, 0);
    gfx_Begin();
    gfx_FillScreen(gfx_black);
}


void Style_Finalize(void)
{
    /* Finish the graphics */
    gfx_End();
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
