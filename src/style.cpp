#include "style.h"
#include <fontlibc.h>
#include "misc.h"
#include "forms/textmanager.h"

unsigned int GroupDelimiterWidth;

/**
 * The FontId of the last font set by a call to Style_SetSomeFont().
 */
FontId_t CurrentFont;


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
    if (CurrentFont != FONT_SMALL_PROP_ALIGNED)
        saveTarget->FontId = Forms::FontManager::GetCurrentFont();
    else
        saveTarget->FontId = CurrentFont;
}


void Style_RestoreTextWindow(CharTextWindow_t* window)
{
    fontlib_SetWindow(window->X, window->Y, window->Width, window->Height);
    fontlib_SetCursorPosition(window->CursorX, window->CursorY);
    Style_SetFont(window->FontId);
}


void Style_SetLargeFontMono(void)
{
    Forms::FontManager::SetFont(FONT_LARGE_MONO);
    CurrentFont = FONT_LARGE_MONO;
}


void Style_SetLargeFontProp(void)
{
    Forms::FontManager::SetFont(FONT_LARGE_PROP);
    CurrentFont = FONT_LARGE_PROP;
}


void Style_SetSmallFontProp(void)
{
    Forms::FontManager::SetFont(FONT_SMALL_PROP);
    CurrentFont = FONT_SMALL_PROP;
}


void Style_SetSmallFontPropBold(void)
{
    Forms::FontManager::SetFont(FONT_SMALL_PROP_BOLD);
    CurrentFont = FONT_SMALL_PROP_BOLD;
}


void Style_SetSmallFontPropAligned(void)
{
    fontlib_font_t* big = Forms::FontManager::GetFont(FONT_LARGE_PROP);
    fontlib_font_t* small = Forms::FontManager::GetFont(FONT_SMALL_PROP);
    unsigned char dh = big->baseline_height - small->baseline_height;
    Forms::FontManager::SetFont(FONT_SMALL_PROP);
    fontlib_SetLineSpacing(dh, big->height - dh - small->height);
    CurrentFont = FONT_SMALL_PROP_ALIGNED;
}


void Style_SetFont(uint8_t fontId)
{
    switch (fontId)
    {
        case FONT_LARGE_PROP:
            Style_SetLargeFontProp();
            break;
        case FONT_LARGE_MONO:
            Style_SetLargeFontMono();
            break;
        case FONT_SMALL_PROP:
            Style_SetSmallFontProp();
            break;
        case FONT_SMALL_PROP_BOLD:
            Style_SetSmallFontPropBold();
            break;
        case FONT_SMALL_PROP_ALIGNED:
            Style_SetSmallFontPropAligned();
            break;
    }
}
