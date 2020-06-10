#include "inputbigint.h"
#include <tice.h>
#include <stdbool.h>
#include <stdlib.h>

/**
 * Number currently being entered by user.
 */
static BigInt_t CurrentInput;

/**
 * Temporary variable used for input.
 */
static BigInt_t Addend;

/**
 * The literal number ten.
 */
static BigInt_t const Ten = { 0xA, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, };

/**
 * true if user is entering number, false if not
 */
static bool EntryActive = false;

/**
 * Maps key codes to their hexadecimal value.
 */
static unsigned char const NumberMap[] = 
{
    0xFF, /* 0 */
    0xFF, /* sk_Down             0x01 */
    0xFF, /* sk_Left             0x02 */
    0xFF, /* sk_Right            0x03 */
    0xFF, /* sk_Up               0x04 */
    0xFF, 0xFF, 0xFF, 0xFF, /* 5, 6, 7, 8 */
    0xFF, /* sk_Enter            0x09 */
    0xFF, /* sk_Add              0x0A */
    0xFF, /* sk_Sub              0x0B */
    0xFF, /* sk_Mul              0x0C */
    0xFF, /* sk_Div              0x0D */
    0xFF, /* sk_Power            0x0E */
    0xFF, /* sk_Clear            0x0F */
    0xFF, /* 10 */
    0xFF, /* sk_Chs              0x11 */
    3, /* sk_3                0x12 */
    6, /* sk_6                0x13 */
    9, /* sk_9                0x14 */
    0xFF, /* sk_RParen           0x15 */
    0xFF, /* sk_Tan              0x16 */
    0xFF, /* sk_Vars             0x17 */
    0xFF, /* 18 */
    0xFF, /* sk_DecPnt           0x19 */
    2, /* sk_2                0x1A */
    5, /* sk_5                0x1B */
    8, /* sk_8                0x1C */
    0xFF, /* sk_LParen           0x1D */
    0xF, /* sk_Cos              0x1E */
    0xC, /* sk_Prgm             0x1F */
    0xFF, /* sk_Stat             0x20 */
    0xFF, /* sk_0                0x21 */
    1, /* sk_1                0x22 */
    4, /* sk_4                0x23 */
    7, /* sk_7                0x24 */
    0xFF, /* sk_Comma            0x25 */
    0xE, /* sk_Sin              0x26 */
    0xB, /* sk_Apps             0x27 */
    0xFF, /* sk_GraphVar         0x28 */
    0xFF, /* 29 */
    0xFF, /* sk_Store            0x2A */
    0xFF, /* sk_Ln               0x2B */
    0xFF, /* sk_Log              0x2C */
    0xFF, /* sk_Square           0x2D */
    0xD, /* sk_Recip            0x2E */
    0xA, /* sk_Math             0x2F */
    0xFF, /* sk_Alpha            0x30 */
    0xFF, /* sk_Graph            0x31 */
    0xFF, /* sk_Trace            0x32 */
    0xFF, /* sk_Zoom             0x33 */
    0xFF, /* sk_Window           0x34 */
    0xFF, /* sk_Yequ             0x35 */
    0xFF, /* sk_2nd              0x36 */
    0xFF, /* sk_Mode             0x37 */
    0xFF /* sk_Del              0x38 */
};


void GetBigInt(BigInt_t* n)
{
    BigIntCopyFromTo(&CurrentInput, n);
}


bool GetBigInt_SendKey(sk_key_t k)
{
    BigInt_t temp, temp2;
    unsigned char digit = NumberMap[k];
    if (digit < 16)
    {
        Addend.d[0] = digit;
        switch (Settings.PrimaryBase)
        {
            case BINARY:
                if (digit > 1)
                    return false;
                BigIntShiftBitInOnLeft(&CurrentInput, digit);
                break;
            case OCTAL:
                if (digit > 7)
                    return false;
                BigIntShiftLeft(&CurrentInput);
                BigIntShiftLeft(&CurrentInput);
                BigIntShiftLeft(&CurrentInput);
                BigIntAdd(&CurrentInput, &Addend);
                break;
            case DECIMAL:
                if (digit > 9)
                    return false;
                BigIntMultiply(&CurrentInput, &Ten, &temp);
                BigIntCopyFromTo(&temp, &CurrentInput);
                BigIntAdd(&CurrentInput, &Addend);
                break;
            case HEXADECIMAL:
                BigIntShiftLeft(&CurrentInput);
                BigIntShiftLeft(&CurrentInput);
                BigIntShiftLeft(&CurrentInput);
                BigIntShiftLeft(&CurrentInput);
                BigIntAdd(&CurrentInput, &Addend);
                break;
        }
        EntryActive = true;
        return true;
    }
    else if (k == sk_Del)
    {
        if (!EntryActive)
            return false;
        switch (Settings.PrimaryBase)
        {
            case HEXADECIMAL:
                BigIntShiftRight(&CurrentInput);
            case OCTAL:
                BigIntShiftRight(&CurrentInput);
                BigIntShiftRight(&CurrentInput);
            case BINARY:
                BigIntShiftRight(&CurrentInput);
                break;
            case DECIMAL:
                BigIntDivide(&CurrentInput, &Ten, &temp, &temp2);
                BigIntCopyFromTo(&temp, &CurrentInput);
                break;
        }
        GetBigInt_Redraw();
        return true;
    }
    return false;
}


bool GetBigInt_IsActive(void)
{
    return EntryActive;
}


void GetBigInt_Reset(void)
{
    BigIntSetToZero(&CurrentInput);
    EntryActive = false;
}


void GetBigInt_Redraw(void)
{
    Coord_t nextLoc;
    if (!EntryActive)
        return;
    fontlib_HomeUp();
    Format_PrintInBase(&CurrentInput, Settings.PrimaryBase);
    Style_SaveCursor(&nextLoc);
    fontlib_HomeUp();
    fontlib_DrawString("#");
    Style_RestoreCursor(&nextLoc);
}
