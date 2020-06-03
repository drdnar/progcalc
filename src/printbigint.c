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


static char* printBinCh[] = 
{
    Format_NumberBuffer + (BIG_INT_SIZE - 4) * 8,
    Format_NumberBuffer + (BIG_INT_SIZE - 8) * 8,
    Format_NumberBuffer + (BIG_INT_SIZE - 16) * 8,
};

unsigned char Format_PrintBin(BigInt_t* n)
{
    /*CharTextWindow_t oldWindow;*/
    unsigned char h, i, j;
    char* ch;
    unsigned int x = fontlib_GetCursorX();
    uint8_t y = fontlib_GetCursorY();
    uint8_t fontHeight = fontlib_GetCurrentFontHeight();
    /*Style_SaveTextWindow(&oldWindow);*/
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
        fontlib_SetCursorPosition(x, y);
        for (i = 4; i > 0; i--)
        {
            fontlib_DrawStringL(ch, BIN_GROUPING);
            ch = fontlib_GetLastCharacterRead() + 1;
            /*while (!os_GetCSC());*/
            if (i != 1)
                fontlib_DrawGlyph(' ');
        }
        fontlib_Newline();
        y += fontHeight;
    }

    /*Style_RestoreTextWindow(&oldWindow);*/
    return Format_BinSize.y;
}


static uint8_t const printDecCopy[] =
{
    4, 8, 16,
};

/*
128 -> e38
64 -> e19
32 -> e9
*/
static char const printDecInit[] = "000 000 000 000 000 000 000 000 000 000 000 000 000";

static char* const printDecStart[] =
{
    Format_NumberBuffer + sizeof(printDecInit) - 13 - 1,
    Format_NumberBuffer + sizeof(printDecInit) - 26 - 1,
    Format_NumberBuffer + sizeof(printDecInit) - 51 - 1,
};

#define formatBufMid (Format_NumberBuffer + MAX_FORMATTED_NUMBER_SIZE / 2)

unsigned char Format_PrintDec(BigInt_t* n)
{
    unsigned char i, j, expected;
    char* ch, *src;
    unsigned int copyBytes;
    unsigned int originalX = fontlib_GetCursorX();
    BigIntToStringBin(n, Format_NumberBuffer);
    BigIntSetToZero(&tempn);
    
    memcpy(&tempn, n, printDecCopy[Settings.DisplayBits]);
    strcpy(Format_NumberBuffer, printDecInit);
    src = BigIntToString(&tempn, formatBufMid);
    /* Somewhat dubious pointer arithmetic, but should be well-defined since
       the output is part of the same array as the input. */
    copyBytes = src-- - formatBufMid;
    ch = Format_NumberBuffer + sizeof(printDecInit) - 2;
    
    while (copyBytes --> 0)
    {
        if (*src != '0')
            src--;
        *ch-- = *src--;
    }
    
    ch = printDecStart[Settings.DisplayBits];
    for ( /* Loop initialized above * ; i > 0; i--)
    {
        fontlib_DrawStringL(ch, DEC_GROUPING);
        ch = fontlib_GetLastCharacterRead() + 1;
        if (i != 1)
            fontlib_DrawGlyph(' ');
    }
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
    BigIntToStringHex(n, Format_NumberBuffer);

    for (i = printHexI[Settings.DisplayBits], ch = printHexCh[Settings.DisplayBits]; i > 0; i--)
    {
        fontlib_DrawStringL(ch, HEX_GROUPING);
        ch = fontlib_GetLastCharacterRead() + 1;
        if (i != 1)
            fontlib_DrawGlyph(' ');
    }
    fontlib_Newline();
    return Format_HexSize.y;
}