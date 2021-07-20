#include "textmanager.h"
#include <fontlibc.h>
#include <graphx.h>

extern "C" void InitializationError(const char* message);

using namespace Forms;

FontId FontManager::CurrentFont;
FontManager FontManager::instance;
fontlib_font_t* FontManager::fonts[MAX_FONTS];
bool FontManager::initial_loading;
bool FontManager::initialized = false;


void TextWindow::Save()
{
    X = fontlib_GetWindowXMin();
    Y = fontlib_GetWindowYMin();
    Width = fontlib_GetWindowWidth();
    Height = fontlib_GetWindowHeight();
    CursorX = fontlib_GetCursorX();
    CursorY = fontlib_GetCursorY();
    FontId = FontManager::CurrentFont;
}


void TextWindow::Restore() const
{
    fontlib_SetWindow(X, Y, Width, Height);
    fontlib_SetCursorPosition(CursorX, CursorY);
    FontManager::SetFont(FontId);
}


void FontManager::ReloadFonts()
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


void FontManager::load_fonts()
{
    if (initialized)
        return;
    initialized = true;
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


void FontManager::EnsureInitialized()
{
    load_fonts();
}


void CursorLoc::Save()
{
    x = fontlib_GetCursorX();
    y = fontlib_GetCursorY();
}


void CursorLoc::Restore() const
{
    fontlib_SetCursorPosition(x, y);
}


void WordWrap::GetTextDimensions(const char* string, Coord& size, x_t max_width)
{
    WindowSaver saver;
    if (max_width)
    {
        fontlib_SetWindow(0, 0, max_width, LCD_HEIGHT);
        fontlib_HomeUp();
    }
    x_t width = 0, initial_x, line_width;
    y_t height = 0;
    y_t font_height = fontlib_GetCurrentFontHeight();
    unsigned char ch;
    unsigned char first = fontlib_GetFirstPrintableCodePoint();
    do
    {
        height += font_height;
        initial_x = fontlib_GetCursorX();
        string = PrintLine(string, true, &line_width);
        line_width = line_width - initial_x;
        if (line_width > width)
            width = line_width;
        ch = (unsigned char)*string;
        if (ch == '\n')
        {
            string++;
            fontlib_SetCursorPosition(saver.Window.X, saver.Window.CursorY);
        }
    } while (ch != 0 && ch > first);
    size.x = width;
    size.y = height;
}


/**
 * I should probably have documented that fontlib_Newline() returns nonzero on
 * failure.
 */
#define fontlib_Newline (*(unsigned char (*)(void))&fontlib_Newline)
const char* WordWrap::Print(const char* string)
{
    auto nlopts = fontlib_GetNewlineOptions();
    unsigned char first_printable = (unsigned char)fontlib_GetFirstPrintableCodePoint();
    fontlib_SetNewlineOptions(FONTLIB_AUTO_CLEAR_TO_EOL | FONTLIB_ENABLE_AUTO_WRAP);
    unsigned char ch;
    do
    {
        string = PrintLine(string, false);
        ch = (unsigned char)*string;
        if (ch < first_printable)
        {
            if (ch == '\n')
            {
                string++;
                if (fontlib_Newline() > 0)
                    break;
            }
            else
                break;
        }
        else
            if (fontlib_Newline() > 0)
                break;
    } while (ch != 0 && ch >= first_printable);
    fontlib_SetNewlineOptions(nlopts);
    return string;
}


const char* WordWrap::PrintLine(const char* string, bool fake_print, x_t* final_x)
{
    char old_stop = fontlib_GetAlternateStopCode();
    x_t left = fontlib_GetWindowXMin();
    x_t width = fontlib_GetWindowWidth();
    x_t right = left + width;
    x_t str_width;
    x_t x = fontlib_GetCursorX();
    unsigned char first_printable = (unsigned char)fontlib_GetFirstPrintableCodePoint();
    unsigned char c;
    x_t space_width = fontlib_GetGlyphWidth(' ');
    if (first_printable == '\0')
        first_printable = '\1';
    fontlib_SetAlternateStopCode(' ');
    do
    {
        /* Check if the next word can fit on the current line */
        str_width = fontlib_GetStringWidth(string);
        if (x + str_width <= right)
        {
            if (!fake_print)
                x = fontlib_DrawString(string);
            else
                x += str_width;
            string = fontlib_GetLastCharacterRead();
        }
        else
        {
            /* If the word is super-long such that it won't fit in the window,
             * then forcibly print it starting on a new line. */
            if (str_width != 0)
            {
                if (str_width > width && x == left)
                    if (!fake_print)
                    {
                        x = fontlib_DrawString(string);
                        string = fontlib_GetLastCharacterRead();
                    }
                    else
                    {
                        do
                            x += fontlib_GetGlyphWidth(*string++);
                        while (x < right);
                        string--;
                        break;
                    }
                else
                    break;
            }
            /* If the width returned was zero, that means either another space
             * is waiting to be printed, which will be handled below; or a
             * control code is next, which also will be handled below.  This can
             * occur, for example, if a control code immediately follows a
             * space. */
        }
        /* Now we need to deal with why the last word was terminated. */
        c = (unsigned char)(*string);
        /* If it's a control code, we either process a newline or give up. */
        if (c < first_printable)
        {
/*            if (c == ZERO_WIDTH_SPACE)
                string++;
            else */if (c == '\t')
            {
                string++;
                x += 16;
                x &= 0xFFFFF0;
                if (!fake_print)
                {
                    fontlib_ClearEOL();
                    fontlib_SetCursorPosition(x, fontlib_GetCursorY());
                }
            }
            else
                break;
        }
        /* If it's a space, we need to process that manually since DrawString
         * won't handle it because we set space as a stop code. */
        if (c == ' ')
        {
            string++;
            /* We do actually need to check if there's space to print the
             * space. */
            if (x + space_width < right)
            {
                if (!fake_print)
                    fontlib_DrawGlyph(' ');
                x += space_width;
            }
            else
            {
                /* If there isn't room, we need to eat the space; it would look
                 * weird to print a space at the start of the next line. 
                 * However, we do not eat ALL the spaces if there's more than
                 * one, just the first one or two. */
                if (*string == ' ') /* Take care of possible second space. */
                    string++;
                break;
            }
        }
    } while (true);
    if (!fake_print)
        fontlib_ClearEOL();
    fontlib_SetAlternateStopCode(old_stop);
    if (final_x)
        *final_x = x;
    return string;
}
