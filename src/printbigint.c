#include "printbigint.h"
#include "settings.h"
#include <fontlibc.h>
#include <string.h>

char Format_NumberBuffer[MAX_FORMATTED_NUMBER_SIZE];
BigInt_t tempn;


static const CharCoord_t displaySizes[3][4] = 
{
    // SHOW_32
    {
        {36, 1}, // BINARY
        {16, 1}, // OCTAL
        {13, 1}, // DECIMAL
        { 9, 1}, // HEXADECIMAL
    },
    // SHOW_64
    {
        {36, 2}, // BINARY
        {33, 1}, // OCTAL
        {28, 1}, // DECIMAL
        {19, 1}, // HEXADECIMAL
    },
    // SHOW_128
    {
        {36, 4}, // BINARY
        {33, 2}, // OCTAL
        {28, 2}, // DECIMAL
        {39, 1}, // HEXADECIMAL
    },
};


void Format_ConfigureDisplaySizes(void)
{
    /* So, uh, I used to have this:
    Format_BinSize = displaySizes[Settings.DisplayBits][BINARY];
    Format_OctSize = displaySizes[Settings.DisplayBits][OCTAL];
    Format_DecSize = displaySizes[Settings.DisplayBits][DECIMAL];
    Format_HexSize = displaySizes[Settings.DisplayBits][HEXADECIMAL];
    and ZDS generated each array index separately instead of seeing they're related so now I'm doing this.
    */
    CharCoord_t* ptr = &displaySizes[Settings.DisplayBits][BINARY];
    Format_BinSize = *ptr++;
    Format_OctSize = *ptr++;
    Format_DecSize = *ptr++;
    Format_HexSize = *ptr;
}


static CharTextWindow_t oldWindow;

static void windowize(void)
{
    unsigned int x = fontlib_GetCursorX();
    uint8_t y = fontlib_GetCursorY();
    Style_SaveTextWindow(&oldWindow);
    fontlib_SetWindow(x, y, fontlib_GetWindowWidth() - x, fontlib_GetWindowHeight() - y);
}


static void unwindowize(void)
{
    uint8_t y = fontlib_GetCursorY();
    Style_RestoreTextWindow(&oldWindow);
    fontlib_SetCursorPosition(fontlib_GetCursorX(), y);
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


unsigned char Format_PrintBin(BigInt_t* n)
{
    unsigned char h, i, j;
    char* ch;
    windowize();
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
    for (h = Format_BinSize.y; h > 0; h--)
    {
        for (i = 4; i > 0; i--)
        {
            fontlib_DrawStringL(ch, BIN_GROUPING);
            ch = fontlib_GetLastCharacterRead() + 1;
            if (i != 1)
                printNumSep();
        }
        fontlib_Newline();
    }

    unwindowize();
    return Format_BinSize.y;
}


static size_t const printDecCopy[] =
{
    4, 8, 16,
};

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

unsigned char Format_PrintDec(BigInt_t* n)
{
    unsigned char i, j, expected;
    unsigned char group, subgroup, actualDigits;
    char* ch, *src;
    size_t copyBytes = printDecCopy[Settings.DisplayBits];
    windowize();

    BigIntToStringBin(n, Format_NumberBuffer);
    BigIntSetToZero(&tempn);
    
    memcpy(&tempn, n, copyBytes);

    src = BigIntToString(&tempn, Format_NumberBuffer);
    /* Somewhat dubious pointer arithmetic, but should be well-defined since
       the output is part of the same array as the input. */
    actualDigits = (unsigned char)(src - Format_NumberBuffer);
    expected = printDecExpectedSize[Settings.DisplayBits];
    group = printDecGroups[Settings.DisplayBits];
    subgroup = printDecInitialDigits[Settings.DisplayBits];

    if (Settings.DisplayBits == SHOW_128)
        /*fontlib_SetCursorPosition(fontlib_GetCursorX() + DEC_GROUPING * CHAR_WIDTH + GroupDelimiterWidth, fontlib_GetCursorY());*/
    {
        fontlib_DrawString("   ");
        printNumSep();
    }
    while (expected-- > actualDigits)
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
    return Format_DecSize.y;
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

unsigned char Format_PrintHex(BigInt_t* n)
{
    /*unsigned int originalX = fontlib_GetCursorX();*/
    unsigned char i, j;
    char* ch;
    windowize();
    BigIntToStringHex(n, Format_NumberBuffer);

    for (i = printHexI[Settings.DisplayBits], ch = printHexCh[Settings.DisplayBits]; i > 0; i--)
    {
        fontlib_DrawStringL(ch, HEX_GROUPING);
        ch = fontlib_GetLastCharacterRead() + 1;
        if (i != 1)
            printNumSep();
    }
    fontlib_Newline();
    unwindowize();
    return Format_HexSize.y;
}