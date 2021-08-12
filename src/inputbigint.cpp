#include <tice.h>
#include <stdbool.h>
#include <string.h>
#include <fontlibc.h>
#include "inputbigint.h"
#include "printbigint.h"
#include "bigint.h"
#include "settings.h"
#include "rpnui.h"
#include "forms/textmanager.h"


using namespace Forms;

#define rpnui (*((RPN_UI*)parent))

const BigInt_t BigIntInput::Ten = { 0xA, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, };

const BigInt_t BigIntInput::MaxDecimal = { 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x19 };

const BigInt_t BigIntInput::MaxShow32 = { 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

const BigInt_t BigIntInput::MaxShow64 = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, 0, 0, 0, 0, 0, 0 };

BigInt_t BigIntInput::CurrentInput;

BigInt_t BigIntInput::Addend;

const unsigned char BigIntInput::NumberMap[] = 
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



bool BigIntInput::SendInput(Message& message)
{
    if (message.Id != MESSAGE_KEY)
        return false;
    sk_key_t k = (sk_key_t)message.ExtendedCode;
    if (k > sk_Del)
        return false;
    BigInt_t temp, temp2;
    unsigned char msb;
    unsigned char digit = NumberMap[k];
    if (digit < 16)
    {
        Addend.d[0] = digit;
        msb = CurrentInput.d[BIG_INT_SIZE - 1];
        BigIntCopyFromTo(&CurrentInput, &temp2);
        switch (Settings::GetPrimaryBase())
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
        if ((Settings::GetDisplayBits() == SHOW_32 && BigIntCompare(&CurrentInput, &MaxShow32) > 0)
            || (Settings::GetDisplayBits() == SHOW_64 && BigIntCompare(&CurrentInput, &MaxShow64) > 0))
        {
            BigIntCopyFromTo(&temp2, &CurrentInput);
            return false;
        }
        if (entryActive)
            SetDirty();
        else
            beginEntry();
        return true;
    }
    else if (k == sk_Del)
    {
        if (!entryActive)
            return false;
        switch (Settings::GetPrimaryBase())
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
        SetDirty();
        return true;
    }
    else if (k == sk_Clear)
    {
        if (!entryActive)
            return false;
        Reset();
        return true;
    }
    return false;
}


Status BigIntInput::Paint()
{
    if (!dirty)
        return Status::Success;
    dirty = false;
    if (!entryActive)
        return Status::Success;
    fontlib_SetWindow(x, y, width, height);
    fontlib_HomeUp();
    Format_PrintEntryWithAlwaysShows(&CurrentInput);
    fontlib_HomeUp();
    fontlib_DrawString("#");
    return Status::Success;
}


void BigIntInput::Layout()
{
    if (entryActive)
    {
        width = rpnui.GetWidth();
        height = Format_GetEntryWithAlwaysShowsHeight();
        x = rpnui.GetX();
        y = rpnui.GetY() + rpnui.GetHeight() - height;
    }
    else
    {
        //y += height;
        height = 0;
    }
}


void BigIntInput::beginEntry()
{
    if (entryActive)
        return;
    entryActive = true;
    Layout();
    rpnui.Layout();
}


void BigIntInput::endEntry()
{
    if (!entryActive)
        return;
    entryActive = false;
    Layout();
    rpnui.Layout();
}


bool BigIntInput::SetEntry(BigInt_t* n)
{
    if (entryActive
        || (Settings::GetDisplayBits() == SHOW_32 && BigIntCompare(n, &MaxShow32) > 0)
        || (Settings::GetDisplayBits() == SHOW_64 && BigIntCompare(n, &MaxShow64) > 0))
        return false;
    beginEntry();
    BigIntCopyFromTo(n, &CurrentInput);
    return true;
}


void BigIntInput::Reset()
{
    BigIntSetToZero(&CurrentInput);
    endEntry();
}


void BigIntInput::GetEntry(BigInt_t* n)
{
    BigIntCopyFromTo(&CurrentInput, n);
}
