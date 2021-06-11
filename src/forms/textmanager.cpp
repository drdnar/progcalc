#include "textmanager.h"
#include <fontlibc.h>
#include <graphx.h>

extern "C" void InitializationError(const char* message);

using namespace Forms;

FontId FontManager::CurrentFont;
FontManager FontManager::instance;
fontlib_font_t* FontManager::fonts[MAX_FONTS];
bool FontManager::initial_loading;

void TextWindow::Save(void)
{
    X = fontlib_GetWindowXMin();
    Y = fontlib_GetWindowYMin();
    Width = fontlib_GetWindowWidth();
    Height = fontlib_GetWindowHeight();
    CursorX = fontlib_GetCursorX();
    CursorY = fontlib_GetCursorY();
    FontId = FontManager::CurrentFont;
}


void TextWindow::Restore(void) const
{
    fontlib_SetWindow(X, Y, Width, Height);
    fontlib_SetCursorPosition(CursorX, CursorY);
    FontManager::SetFont(FontId);
}


void FontManager::ReloadFonts(void)
{
    const FontDescriptor* desc = FontsList;
    fontlib_font_t** font = fonts;
    for (unsigned char i = FontsCount; i > 0; i--, desc++, font++)
    {
        *font = fontlib_GetFontByStyle(desc->PackName, desc->MinSize, desc->MaxSize, desc->MinWeight, desc->MaxWeight, desc->StyleBitsSet, desc->StyleBitsReset);
        if (*font == nullptr)
            if (initial_loading)
                InitializationError("A required font is missing.");
            /* TODO: handle else */
    }
}


void FontManager::load_fonts(void)
{
    if (FontsCount > MAX_FONTS)
        InitializationError("Configured Forms::FontsCount > Forms::FontManager::MAX_FONTS");
    initial_loading = true;
    ReloadFonts();
    initial_loading = false;
}


void FontManager::SetFont(FontId fontId)
{
    CurrentFont = fontId;
    fontlib_SetFont(fonts[fontId], (fontlib_load_options_t)0);
    unsigned char osa = FontsList[fontId].OverrideSpaceAbove;
    unsigned char osb = FontsList[fontId].OverrideSpaceBelow;
    fontlib_SetLineSpacing(osa == (unsigned char)-1 ? fontlib_GetSpaceAbove() : osa, 
                           osb == (unsigned char)-1 ? fontlib_GetSpaceBelow() : osb);
}


void CursorLoc::Save(void)
{
    x = fontlib_GetCursorX();
    y = fontlib_GetCursorY();
}


void CursorLoc::Restore(void) const
{
    fontlib_SetCursorPosition(x, y);
}
