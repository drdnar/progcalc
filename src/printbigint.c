#include "printbigint.h"
#include "settings.h"
#include <fontlibc.h>
#include <string.h>

char Format_NumberBuffer[MAX_FORMATTED_NUMBER_SIZE];
BigInt_t tempn;


static Coord_t displaySizes[3][4] = 
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
    Coord_t* coord = &displaySizes[0][0];
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
    Coord_t* ptr = &displaySizes[Settings.DisplayBits][BINARY];
    Format_BinSize = *ptr++;
    Format_OctSize = *ptr++;
    Format_DecSize = *ptr++;
    Format_HexSize = *ptr;
}


unsigned char Format_GetNumberHeight(Base_t base)
{
    if (base == NO_BASE)
        return 0;
    return displaySizes[Settings.DisplayBits][base].y;
}


void Format_PrintInBase(BigInt_t* n, Base_t base)
{
    Coord_t home;
    unsigned int x1, x2 = 0;
    unsigned char y1;
    Style_SaveCursor(&home);
    switch (base)
    {
        case BINARY:
            x2 = Format_PrintBin(n);
            break;
        case OCTAL:
            x2 = Format_PrintOct(n);
            break;
        case DECIMAL:
            x2 = Format_PrintDec(n);
            break;
        case HEXADECIMAL:
            x2 = Format_PrintHex(n);
            break;
    }
    gfx_SetColor(fontlib_GetBackgroundColor());
    gfx_FillRectangle_NoClip(home.x, home.y, x2 - home.x, fontlib_GetCursorY() - home.y + fontlib_GetCurrentFontHeight());
}


static CharTextWindow_t oldWindow;


/**
 * Creates a temporary text window of a given width, right-aligned to current window.
 * @param width Width of new window.
 */
static void windowize(unsigned int width)
{
/* TODO: Not sure which way to do this is better.
    unsigned int x = fontlib_GetWindowXMin() + fontlib_GetWindowWidth() - width;
    uint8_t y = fontlib_GetCursorY();
    Style_SaveTextWindow(&oldWindow);
    fontlib_SetWindow(x, y, fontlib_GetWindowWidth() - x, fontlib_GetWindowHeight() - y);
    fontlib_HomeUp();
*/
    unsigned int x;
    uint8_t y;
    Style_SaveTextWindow(&oldWindow);
    x = oldWindow.X + oldWindow.Width - width;
    y = oldWindow.CursorY;
    fontlib_SetWindow(x, y, oldWindow.Width - x, oldWindow.Y + oldWindow.Height - y);
    fontlib_HomeUp();
}


static unsigned int PrintBaseLabel(Base_t base, unsigned char height)
{
    char* baseName;
    bool altHex = false;
    unsigned int x;
    if (base == HEXADECIMAL && Settings.DisplayBits >= SHOW_128)
    {
        altHex = true;
        baseName = "0x";
    }
    else
        baseName = GetBaseShortName(base);
    Style_SetSmallFontPropAligned();
    fontlib_SetLineSpacing(fontlib_GetSpaceAbove(), height - fontlib_GetCurrentFontHeight() + fontlib_GetSpaceBelow());
    x = fontlib_GetCursorX() - fontlib_GetStringWidth(baseName) - (altHex ? 0 : fontlib_GetGlyphWidth(' '));
    fontlib_SetCursorPosition(x, fontlib_GetCursorY());
    fontlib_DrawString(baseName);
    if (!altHex)
        fontlib_DrawGlyph(' ');
    Style_SetLargeFontMono();
    return x;
}


/**
 * Restores text window settings saved by earlier call to windowize().
 * Cursor row will be preserved, but column will be restored to previous cursor column.
 */
static void unwindowize(void)
{
    uint8_t y = fontlib_GetCursorY();
    Style_RestoreTextWindow(&oldWindow);
/* TODO: Not sure which way is better.
    fontlib_SetCursorPosition(fontlib_GetCursorX(), y);
*/
    fontlib_SetCursorPosition(oldWindow.CursorX, y);
}


static void printNumSep(void)
{
    Style_SetLargeFontProp();
    fontlib_DrawGlyph(GROUP_DELIMITER);
    Style_SetLargeFontMono();
}


static char* printBinCh[] = 
{
    Format_NumberBuffer + (BIG_INT_SIZE - 4) * 8,
    Format_NumberBuffer + (BIG_INT_SIZE - 8) * 8,
    Format_NumberBuffer + (BIG_INT_SIZE - 16) * 8,
};


unsigned int Format_PrintBin(BigInt_t* n)
{
    unsigned char h, i, j;
    char* ch;
    unsigned int xreturn;
    windowize(Format_BinSize.x);
    xreturn = PrintBaseLabel(BINARY, Format_BinSize.y);
    BigIntToStringBin(n, Format_NumberBuffer);
    /*switch (Settings.DisplayBits)
    {
        case SHOW_32:
            i = 4;
            ch = FormattedNumberBuffer + (BIG_INT_SIZE - 4) * 8;
            break;
        case SHOW_64:
            i = 8;
            ch = FormattedNumberBuffer + (BIG_INT_SIZE - 8) * 8;
            break;
        case SHOW_128:
            i = 16;
            ch = FormattedNumberBuffer + (BIG_INT_SIZE - 16) * 8;
            break;
    }
    for (; i > 0; i--)*/
    /* Array indexing seems to produce less terrible output than if or switch. */
    ch = printBinCh[Settings.DisplayBits];
    for (h = Format_BinSize.y / CHAR_HEIGHT; h > 0; h--)
    {
        for (i = 4; i > 0; i--)
        {
            fontlib_DrawStringL(ch, BIN_GROUPING);
            ch = fontlib_GetLastCharacterRead() + 1;
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
    memcpy(&tempn, n, printPartialCopy[Settings.DisplayBits]);
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
    unsigned char i, j, expected;
    unsigned char group, subgroup, actualDigits;
    char* ch, *src;
    unsigned int xreturn;
    windowize(Format_DecSize.x);
    xreturn = PrintBaseLabel(DECIMAL, Format_DecSize.y);
    
    partialCopy(n);

    src = BigIntToString(&tempn, Format_NumberBuffer);
    /* Somewhat dubious pointer arithmetic, but should be well-defined since
       the output is part of the same array as the input. */
    actualDigits = (unsigned char)(src - Format_NumberBuffer);
    expected = printDecExpectedSize[Settings.DisplayBits];
    group = printDecGroups[Settings.DisplayBits];
    subgroup = printDecInitialDigits[Settings.DisplayBits];

    if (Settings.DisplayBits == SHOW_128)
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
    unsigned char i, j;
    char* ch;
    unsigned int xreturn;
    windowize(Format_HexSize.x);
    xreturn = PrintBaseLabel(HEXADECIMAL, Format_HexSize.y);
    BigIntToStringHex(n, Format_NumberBuffer);

    for (i = printHexI[Settings.DisplayBits], ch = printHexCh[Settings.DisplayBits]; i > 0; i--)
    {
        fontlib_DrawStringL(ch, HEX_GROUPING);
        ch = fontlib_GetLastCharacterRead() + 1;
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
    bits = Settings.DisplayBits;
    windowize(Format_OctSize.x);
    xreturn = PrintBaseLabel(OCTAL, Format_OctSize.y);
    
    partialCopy(n);
    
    BigIntToStringOct(&tempn, Format_NumberBuffer);

    g = groupings[bits][OCTAL];
    if (bits == SHOW_128)
    {
        lines = 2;
        fontlib_DrawString("     ");
        printNumSep();
        g = 6;
    }
    
    /* I stopped being clever when I wrote this routine and didn't bother with DrawStringL. */
    h = octFirstGroupDigits[bits];
    ch = printOctCh[bits];
    do
    {
        fontlib_DrawGlyph(*ch);
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

