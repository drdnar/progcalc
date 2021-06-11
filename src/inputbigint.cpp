#include <tice.h>
#include <stdbool.h>
#include <string.h>
#include <fontlibc.h>
#include "inputbigint.h"
#include "printbigint.h"
#include "bigint.h"
#include "settings.h"
#include "rpnui.h"

#include <debug.h>

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
 * The largest decimal number than can accept one more nine.
 */
static BigInt_t const MaxDecimal = { 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x19 };

/**
 * Largest possible 32-bit number.
 */
static BigInt_t const MaxShow32 = { 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

/**
 * Largest possible 64-bit number.
 */
static BigInt_t const MaxShow64 = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, 0, 0, 0, 0, 0, 0 };

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
    0, /* sk_0                0x21 */
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


CharTextWindow_t GetBigInt_Window;

void GetBigInt_Reposition(void)
{
    GetBigInt_Window.X = Rpn_Window.X;
    GetBigInt_Window.Width = Rpn_Window.Width;
    GetBigInt_Window.Height = Format_GetNumberHeight(Settings.PrimaryBase);
    GetBigInt_Window.Y = Rpn_Window.Y + Rpn_Window.Height - GetBigInt_Window.Height;
}


bool GetBigInt_SendKey(sk_key_t k)
{
    BigInt_t temp, temp2;
    unsigned char msb;
    unsigned char digit = NumberMap[k];
    if (digit < 16)
    {
        Addend.d[0] = digit;
        msb = CurrentInput.d[BIG_INT_SIZE - 1];
        BigIntCopyFromTo(&CurrentInput, &temp2);
        switch (Settings.PrimaryBase)
        {
            case BINARY:
                if (digit > 1 || msb > 0x7F)
                    return false;
                BigIntShiftBitInOnLeft(&CurrentInput, digit);
                break;
            case OCTAL:
                if (digit > 7 || msb > 0x1F)
                    return false;
                BigIntShiftLeft(&CurrentInput);
                BigIntShiftLeft(&CurrentInput);
                BigIntShiftLeft(&CurrentInput);
                BigIntAdd(&CurrentInput, &Addend);
                break;
            case DECIMAL:
                if (digit > 9)
                    return false;
                if (BigIntCompare(&CurrentInput, &MaxDecimal) > 0)
                    return false;
                BigIntMultiply(&CurrentInput, &Ten, &temp);
                if (BigIntAdd(&temp, &Addend))
                    return false;
                BigIntCopyFromTo(&temp, &CurrentInput);
                break;
            case HEXADECIMAL:
                if (msb > 0xF)
                    return false;
                BigIntShiftLeft(&CurrentInput);
                BigIntShiftLeft(&CurrentInput);
                BigIntShiftLeft(&CurrentInput);
                BigIntShiftLeft(&CurrentInput);
                BigIntAdd(&CurrentInput, &Addend);
                break;
        }
        if ((Settings.DisplayBits == SHOW_32 && BigIntCompare(&CurrentInput, &MaxShow32) > 0)
            || (Settings.DisplayBits == SHOW_64 && BigIntCompare(&CurrentInput, &MaxShow64) > 0))
        {
            BigIntCopyFromTo(&temp2, &CurrentInput);
            return false;
        }
        EntryActive = true;
        GetBigInt_Redraw();
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
    else if (k == sk_Clear)
    {
        if (!EntryActive || Rpn_IsScrollingActive())
            return false;
        GetBigInt_Reset();
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
    CharTextWindow_t oldWindow;
    if (!EntryActive)
        return;
    Style_SaveTextWindow(&oldWindow);
    Style_RestoreTextWindow(&GetBigInt_Window);
    fontlib_HomeUp();
    Format_PrintInBase(&CurrentInput, Settings.PrimaryBase);
    fontlib_HomeUp();
    fontlib_DrawString("#");
    Style_RestoreTextWindow(&oldWindow);
    /* This is an evil typecast that arises from me still sometimes thinking like an assembly programmer. */
    Style_RestoreCursor((Coord_t*)(&GetBigInt_Window));
}
