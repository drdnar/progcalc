#include "printbigint.h"
#include "settings.h"
#include <fontlibc.h>
#include <string.h>
#include "misc.h"
#include "forms/textmanager.h"
#include "forms/inlineoptimizations.h"
#include "forms/calc1252.h"

char Format_NumberBuffer[MAX_FORMATTED_NUMBER_SIZE];
BigInt_t temp_sign;
bool negative_number;
BigInt_t tempn;

Forms::Coord Format_HexSize;
Forms::Coord Format_DecSize;
Forms::Coord Format_OctSize;
Forms::Coord Format_BinSize;

/**
 * Caches the width of a space in the large variable-width font.
 */
static unsigned int GroupDelimiterWidth;

static Forms::Coord displaySizes[3][4] = 
{
    // SHOW_32
    {
        {32 * CHAR_WIDTH, 1 * CHAR_HEIGHT}, // BINARY
        {11 * CHAR_WIDTH, 1 * CHAR_HEIGHT}, // OCTAL
        {10 * CHAR_WIDTH, 1 * CHAR_HEIGHT}, // DECIMAL
        { 8 * CHAR_WIDTH, 1 * CHAR_HEIGHT}, // HEXADECIMAL
    },
    // SHOW_64
    {
        {32 * CHAR_WIDTH, 2 * CHAR_HEIGHT}, // BINARY
        {22 * CHAR_WIDTH, 1 * CHAR_HEIGHT}, // OCTAL
        {20 * CHAR_WIDTH, 1 * CHAR_HEIGHT}, // DECIMAL
        {16 * CHAR_WIDTH, 1 * CHAR_HEIGHT}, // HEXADECIMAL
    },
    // SHOW_128
    {
        {32 * CHAR_WIDTH, 4 * CHAR_HEIGHT}, // BINARY
        {24 * CHAR_WIDTH, 2 * CHAR_HEIGHT}, // OCTAL
        {21 * CHAR_WIDTH, 2 * CHAR_HEIGHT}, // DECIMAL
        {32 * CHAR_WIDTH, 1 * CHAR_HEIGHT}, // HEXADECIMAL
    },
};


static unsigned char const groupings[3][4] =
{
    // SHOW_32
    {
        3, // BINARY
        3, // OCTAL
        3, // DECIMAL
        1, // HEXADECIMAL
    },
    // SHOW_64
    {
        3, // BINARY
        7, // OCTAL
        6, // DECIMAL
        3, // HEXADECIMAL
    },
    // SHOW_128
    {
        3, // BINARY
        7, // OCTAL
        6, // DECIMAL
        7, // HEXADECIMAL
    },
};


void Format_InitDisplaySizes(void)
{
    unsigned char i;
    const unsigned char* spacers = &groupings[0][0];
    Forms::FontManager::SetFont(FONT_LARGE_PROP);
    GroupDelimiterWidth = fontlib_GetGlyphWidth(GROUP_DELIMITER);
    Forms::FontManager::SetFont(FONT_LARGE_MONO);
    Forms::Coord* coord = &displaySizes[0][0];
    for (i = 0; i < (SHOW_128 + 1) * (HEXADECIMAL + 1); i++, coord++, spacers++)
        coord->x += *spacers * GroupDelimiterWidth;
}


void Format_ConfigureDisplaySizes(void)
{
    /* So, uh, I used to have this:
    Format_BinSize = displaySizes[Settings.DisplayBits][BINARY];
    Format_OctSize = displaySizes[Settings.DisplayBits][OCTAL];
    Format_DecSize = displaySizes[Settings.DisplayBits][DECIMAL];
    Format_HexSize = displaySizes[Settings.DisplayBits][HEXADECIMAL];
    and ZDS generated each array index separately instead of seeing they're related so now I'm doing this.
    */
    Forms::Coord* ptr = &displaySizes[Settings::GetDisplayBits()][BINARY];
    Format_BinSize = *ptr++;
    Format_OctSize = *ptr++;
    Format_DecSize = *ptr++;
    Format_HexSize = *ptr;
}


unsigned char Format_GetNumberHeight(Base_t base)
{
    if (base == NO_BASE)
        return 0;
    return displaySizes[Settings::GetDisplayBits()][base].y;
}


static void print_in_base(BigInt_t* n, Base_t base)
{
    auto x = fontlib_GetCursorX();
    auto y = fontlib_GetCursorY();
    Format_PrintInBase(n, base);
    fontlib_SetCursorPosition(x, y + Format_GetNumberHeight(base));
}


unsigned char Format_GetEntryHeight()
{
    return Format_GetNumberHeight(Settings::GetPrimaryBase())
        + Format_GetNumberHeight(Settings::GetSecondaryBase());
}

void Format_PrintEntry(BigInt_t* n)
{
    print_in_base(n, Settings::GetPrimaryBase());
    if (Settings::GetSecondaryBase() != NO_BASE)
        print_in_base(n, Settings::GetSecondaryBase());
}


static bool base_used(Base_t base)
{
    return Settings::GetPrimaryBase() == base || Settings::GetSecondaryBase() == base;
}


unsigned char Format_GetEntryWithAlwaysShowsHeight()
{
    unsigned char height = Format_GetNumberHeight(Settings::GetPrimaryBase())
        + Format_GetNumberHeight(Settings::GetSecondaryBase());
    for (unsigned char i = 0; i < NO_BASE; i++)
        if (!base_used(i) && Settings::AlwaysShow(i))
            height += Format_GetNumberHeight(i);
    return height;
}


void Format_PrintEntryWithAlwaysShows(BigInt_t* n)
{
    Format_PrintEntry(n);
    for (unsigned char i = 0; i < NO_BASE; i++)
        if (!base_used(i) && Settings::AlwaysShow(i))
            print_in_base(n, i);
}


void Format_PrintInBase(BigInt_t* n, Base_t base)
{
    unsigned int x = 0;
    Forms::CursorLoc cursor;
    cursor.Save();
    switch (base)
    {
        case BINARY:
            x = Format_PrintBin(n);
            break;
        case OCTAL:
            x = Format_PrintOct(n);
            break;
        case DECIMAL:
            x = Format_PrintDec(n);
            break;
        case HEXADECIMAL:
            x = Format_PrintHex(n);
            break;
    }
    gfx_SetColor(fontlib_GetBackgroundColor());
    gfx_FillRectangle_NoClip(cursor.x, cursor.y, x - cursor.x, fontlib_GetCursorY() - cursor.y + fontlib_GetCurrentFontHeight());
}


static Forms::TextWindow oldWindow;


/**
 * Creates a temporary text window of a given width, right-aligned to current window.
 * Furthermore, if signed mode is active and the number is negative, then sets up
 * stuff for displaying the number.
 * @param width Width of new window.
 * @param n Number about to be displayed
 */
static void windowize(unsigned int width, BigInt_t* n)
{
    unsigned int x;
    uint8_t y;
    oldWindow.Save();
    x = oldWindow.X + oldWindow.Width - width;
    y = oldWindow.CursorY;
    fontlib_SetWindow(x, y, oldWindow.Width - x, oldWindow.Y + oldWindow.Height - y);
    fontlib_HomeUp();
    // Handle sign stuff
    BigIntCopyFromTo(n, &temp_sign);
    if (Settings::GetSignedMode() && BigIntGetSign(&temp_sign) < 0)
    {
        negative_number = true;
        BigIntNegate(&temp_sign);
    }
    else
        negative_number = false;
}


static unsigned int PrintBaseLabel(Base_t base, unsigned char height)
{
    const char* baseName;
    bool altHex = false;
    unsigned int x;
    if (base == HEXADECIMAL && Settings::GetDisplayBits() >= SHOW_128)
    {
        altHex = true;
        baseName = "0x";
    }
    else
        baseName = BaseShortNames.Get(base);
    fontlib_font_t* big = Forms::FontManager::GetFont(FONT_LARGE_PROP);
    fontlib_font_t* small = Forms::FontManager::GetFont(FONT_SMALL_PROP);
    unsigned char dh = big->baseline_height - small->baseline_height;
    unsigned int minus_width;
    if (altHex)
    {
        Forms::FontManager::SetFont(FONT_LARGE_PROP);
        minus_width = fontlib_GetGlyphWidth('-');
    }
    else
    {
        Forms::FontManager::SetFont(FONT_LARGE_MONO);
        minus_width = fontlib_GetGlyphWidth(CALC1252_MINUS_CHAR);
    }
    Forms::FontManager::SetFont(FONT_SMALL_PROP);
    fontlib_SetLineSpacing(dh, big->height - dh - small->height);
    fontlib_SetLineSpacing(fontlib_GetSpaceAbove(), height - fontlib_GetCurrentFontHeight() + fontlib_GetSpaceBelow());
    x = fontlib_GetCursorX() - fontlib_GetStringWidth(baseName)
        - (!altHex ? fontlib_GetGlyphWidth(' ') : 0)
        - (Settings::GetSignedMode() && (!altHex || negative_number) ? minus_width : 0);
    fontlib_SetCursorPosition(x, fontlib_GetCursorY());
    fontlib_DrawString(baseName);
    if (!altHex)
        fontlib_DrawGlyph(' ');
    if (Settings::GetSignedMode())
    {
        dh = fontlib_GetCurrentFontHeight();
        if (altHex)
            Forms::FontManager::SetFont(FONT_LARGE_PROP);
        else
            Forms::FontManager::SetFont(FONT_LARGE_MONO);
        dh = dh - fontlib_GetCurrentFontHeight() + fontlib_GetSpaceBelow();
        fontlib_SetLineSpacing(fontlib_GetSpaceAbove(), dh);
        if (negative_number)
        {
            if (altHex)
                fontlib_DrawGlyph('-');
            else
                fontlib_DrawGlyph(CALC1252_MINUS_CHAR);
        }
        else
            if (!altHex)
                fontlib_DrawGlyph(' ');
    }
    Forms::FontManager::SetFont(FONT_LARGE_MONO);
    return x;
}


/**
 * Restores text window settings saved by earlier call to windowize().
 * Cursor row will be preserved, but column will be restored to previous cursor column.
 */
static void unwindowize(void)
{
    uint8_t y = fontlib_GetCursorY();
    oldWindow.Restore();
    fontlib_SetCursorPosition(oldWindow.CursorX, y);
    fontlib_SetForegroundColor(COLOR_FOREGROUND);
}


static void printNumSep(void)
{
    Forms::FontManager::SetFont(FONT_LARGE_PROP);
    fontlib_DrawGlyph(GROUP_DELIMITER);
    Forms::FontManager::SetFont(FONT_LARGE_MONO);
}


static char* printBinCh[] = 
{
    Format_NumberBuffer + (BIG_INT_SIZE - 4) * 8,
    Format_NumberBuffer + (BIG_INT_SIZE - 8) * 8,
    Format_NumberBuffer + (BIG_INT_SIZE - 16) * 8,
};


bool zeros;
static inline void resetZeros()
{
    zeros = true;
    if (Settings::GrayLeadingZeros())
        fontlib_SetForegroundColor(COLOR_ZERO);
}

static void printdigit(char d)
{
    if (zeros && d != '0')
    {
        zeros = true;
        fontlib_SetForegroundColor(COLOR_FOREGROUND);
    }
    fontlib_DrawGlyph(d);
}

static const char* printdigits(const char* s, unsigned char count)
{
    while (count --> 0)
    {
        auto d = *s++;
        if (zeros && d != '0')
        {
            zeros = true;
            fontlib_SetForegroundColor(COLOR_FOREGROUND);
        }
        fontlib_DrawGlyph(d);
    }
    return s;
}


unsigned int Format_PrintBin(BigInt_t* n)
{
    unsigned char h, i;
    const char* ch;
    unsigned int xreturn;
    windowize(Format_BinSize.x, n);
    xreturn = PrintBaseLabel(BINARY, Format_BinSize.y);
    BigIntToStringBin(&temp_sign, Format_NumberBuffer);
    /* Array indexing seems to produce less terrible output than if or switch. */
    ch = printBinCh[Settings::GetDisplayBits()];
    resetZeros();
    for (h = Format_BinSize.y / CHAR_HEIGHT; h > 0; h--)
    {
        for (i = 4; i > 0; i--)
        {
            ch = printdigits(ch, BIN_GROUPING);
            if (i != 1)
                printNumSep();
        }
        if (h > 1)
            fontlib_Newline();
    }

    unwindowize();
    return xreturn;
}


static size_t const printPartialCopy[] =
{
    4, 8, 16,
};

/**
 * Copies part of a number into a temporary buffer for formatting.
 */
static void partialCopy(BigInt_t* n)
{
    BigIntSetToZero(&tempn);
    memcpy_inline_nonzero(&tempn, n, printPartialCopy[Settings::GetDisplayBits()]);
}

/*
128 -> e38 -> 39 characters
64 -> e19 -> 20 characters
32 -> e9 -> 10 characters
*/

static unsigned char const printDecExpectedSize[] =
{
    10, 20, 39,
};

static unsigned char const printDecGroups[] =
{
    3, 6, 12,
};

static unsigned char const printDecInitialDigits[] =
{
    1, 2, 3,
};

unsigned int Format_PrintDec(BigInt_t* n)
{
    unsigned char expected;
    unsigned char group, subgroup, actualDigits;
    char* ch, *src;
    unsigned int xreturn;
    windowize(Format_DecSize.x, n);
    xreturn = PrintBaseLabel(DECIMAL, Format_DecSize.y);
    
    partialCopy(&temp_sign);

    src = BigIntToString(&tempn, Format_NumberBuffer);
    /* Somewhat dubious pointer arithmetic, but should be well-defined since
       the output is part of the same array as the input. */
    actualDigits = (unsigned char)(src - Format_NumberBuffer);
    expected = printDecExpectedSize[Settings::GetDisplayBits()];
    group = printDecGroups[Settings::GetDisplayBits()];
    subgroup = printDecInitialDigits[Settings::GetDisplayBits()];
    if (Settings::GrayLeadingZeros())
        fontlib_SetForegroundColor(COLOR_ZERO);
    if (Settings::GetDisplayBits() == SHOW_128)
    {
        fontlib_DrawString("   ");
        printNumSep();
    }
    while (expected --> actualDigits)
    {
        fontlib_DrawGlyph('0');
        if (--subgroup == 0)
        {
            subgroup = DEC_GROUPING;
            if (group-- == 7)
                fontlib_Newline();
            else
                printNumSep();
        }
    }
    ch = Format_NumberBuffer;
    if (BigIntIsNonZero(&temp_sign))
        fontlib_SetForegroundColor(COLOR_FOREGROUND);
    do
    {
        fontlib_DrawStringL(ch, subgroup);
        ch += subgroup;
        subgroup = DEC_GROUPING;
        if (group == 7)
            fontlib_Newline();
        else if (group)
            printNumSep();
    }
    while (group--);

    unwindowize();
    return xreturn;
}


static uint8_t printHexI[] = 
{
    2, 4, 8,
};

static char* printHexCh[] = 
{
    Format_NumberBuffer + (BIG_INT_SIZE - 4) * 2,
    Format_NumberBuffer + (BIG_INT_SIZE - 8) * 2,
    Format_NumberBuffer + (BIG_INT_SIZE - 16) * 2,
};

unsigned int Format_PrintHex(BigInt_t* n)
{
    unsigned char i;
    const char* ch;
    unsigned int xreturn;
    windowize(Format_HexSize.x, n);
    xreturn = PrintBaseLabel(HEXADECIMAL, Format_HexSize.y);
    BigIntToStringHex(&temp_sign, Format_NumberBuffer);
    resetZeros();
    for (i = printHexI[Settings::GetDisplayBits()], ch = printHexCh[Settings::GetDisplayBits()]; i > 0; i--)
    {
        ch = printdigits(ch, HEX_GROUPING);
        if (i != 1)
            printNumSep();
    }
    unwindowize();
    return xreturn;
}


/**
 * If used with integer arguments, this will compute ceil(x / y).
 * In other words, it divides and rounds up.
 * This will cause problems if you do something degenerate like use zero in the numerator with unsigned numbers..
 * So don't use it at runtime.
 */
#define CEIL_DIV(x, y) ((((x) - 1) / (y)) + 1)

static char* printOctCh[] = 
{
    Format_NumberBuffer + CEIL_DIV(BIG_INT_SIZE * 8, 3) - CEIL_DIV(32, 3),
    Format_NumberBuffer + CEIL_DIV(BIG_INT_SIZE * 8, 3) - CEIL_DIV(64, 3),
    Format_NumberBuffer + CEIL_DIV(BIG_INT_SIZE * 8, 3) - CEIL_DIV(128, 3),
};

static unsigned char octFirstGroupDigits[] =
{
    2, 1, 1
};

unsigned int Format_PrintOct(BigInt_t* n)
{
    /* Here's the formatting we're going for:
                        37 777 777 777
         1 777 777 777 777 777 777 777
             3 777 777 777 777 777 777
       777 777 777 777 777 777 777 777 */
    unsigned char bits, g, h, lines;
    char* ch;
    unsigned int xreturn;
    lines = 1;
    bits = Settings::GetDisplayBits();
    windowize(Format_OctSize.x, n);
    xreturn = PrintBaseLabel(OCTAL, Format_OctSize.y);
    
    partialCopy(&temp_sign);
    
    BigIntToStringOct(&tempn, Format_NumberBuffer);

    g = groupings[bits][OCTAL];
    if (bits == SHOW_128)
    {
        lines = 2;
        fontlib_DrawString("     ");
        printNumSep();
        g = 6;
    }
    
    resetZeros();
    /* I stopped being clever when I wrote this routine and didn't bother with DrawStringL. */
    h = octFirstGroupDigits[bits];
    ch = printOctCh[bits];
    do
    {
        printdigit(*ch);
        if (!(--h))
        {
            if (g--)
                printNumSep();
            else if (--lines)
                fontlib_Newline();
            h = 3;
        }
    }
    while (*(++ch));

    unwindowize();
    return xreturn;
}

