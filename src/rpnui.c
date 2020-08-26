#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <tice.h>
#include <fontlibc.h>
#include <graphx.h>
#include "rpnui.h"
#include "settings.h"
#include "stack.h"
#include "printbigint.h"
#include "style.h"
#include "ui.h"
#include "inputbigint.h"

/**
 * Waiting for user to start typing.
 */
#define RPN_NO_INPUT 0
/**
 * User has started typing a number.
 */
#define RPN_INPUT 1
/**
 * User is scrolling around the stack.
 */
#define RPN_SCROLL 2

/**
 * Current input mode.
 */
static unsigned char EntryMode;

/**
 * Main RPN stack.
 */
static BigIntStack_t* MainStack = NULL;

/**
 * Number currently being entered by user.
 */
static BigInt_t CurrentInput;

static BigInt_t Temp0;
static BigInt_t Temp1;
static BigInt_t Temp2;
static BigInt_t Temp3;

/**
 * Default window for showing stack and user entry.
 */
static CharTextWindow_t StackWindow =
{
    0, 0,
    LCD_WIDTH, LCD_HEIGHT,
    0, 0
};


/**
 * Acquires the user's current input and flushes the input buffer.
 */
static bool AcquireInput(void)
{
    if (!GetBigInt_IsActive())
        return false;
    EntryMode = RPN_NO_INPUT;
    GetBigInt(&CurrentInput);
    GetBigInt_Reset();
    BigIntStack_Push(MainStack, &CurrentInput);
    return true;
}


/**
 * Caches a pointer to the top of the stack after EnsureBinaryOp().
 */
static BigInt_t* topOfStack;
/**
 * Sets up the stack for a binary (two argument) operation.
 * Temp1 will contain the second argument.
 */
static bool EnsureBinaryOp(void)
{
    if (BigIntStack_GetSize(MainStack) < 2)
        return false;
    BigIntStack_Exchange(MainStack);
    BigIntStack_Pop(MainStack, &Temp1);
    topOfStack = BigIntStack_Peek(MainStack);
    return true;
}


/**
 * Draws a stack entry and its label prefix.
 * @param n Index of stack entry number to draw
 * @return true if the number fits into the current window, false if number does not fit.
 */
static bool DrawStackEntry(unsigned int n)
{
    Coord_t home, nextLoc;
    /* Force promotion so there's no overflow issues. */
    unsigned int requiredHeight = Format_GetNumberHeight(Settings.PrimaryBase);
    fontlib_Home();
    Style_SaveCursor(&home);
    if (n >= BigIntStack_GetSize(MainStack) || home.y + requiredHeight > fontlib_GetWindowYMin() + fontlib_GetWindowHeight())
        return false;
    Format_PrintInBase(BigIntStack_Get(MainStack, n, NULL), Settings.PrimaryBase);
    Style_SaveCursor(&nextLoc);
    Style_RestoreCursor(&home);
    fontlib_DrawUInt(n, 2);
    fontlib_DrawGlyph(':');
    Style_RestoreCursor(&nextLoc);
    return true;
}


void Rpn_Redraw(void)
{
    unsigned int index;
    Style_SetLargeFontProp();
    Style_RestoreTextWindow(&StackWindow);
    if (EntryMode == RPN_INPUT)
        GetBigInt_Redraw();
    if (EntryMode == RPN_NO_INPUT || EntryMode == RPN_INPUT)
        index = 0;
    else
        /* TODO: SCROLLING NOT YET IMPLEMENTED */
        index = 0;
    if (BigIntStack_GetSize(MainStack))
        do
            ;
        while (DrawStackEntry(index++));
    else if (EntryMode == RPN_NO_INPUT)
    {
        Style_SetLargeFontProp();
        fontlib_DrawString("(Stack is empty.)");
        fontlib_Newline();
    }
    
    while (fontlib_GetCursorY() > StackWindow.Y)
        fontlib_Newline();
}


void Rpn_SetEntryMode(bool mode)
{
    if (mode)
    {
        if (EntryMode == RPN_INPUT)
            return;
        /* else */
        /* Handle starting entry */
        EntryMode = RPN_INPUT;
        Rpn_Redraw();
    }
    else
    {
        if (EntryMode == RPN_NO_INPUT)
            return;
        /* else */
        /* Handle stopping entry */
        EntryMode = RPN_NO_INPUT;
        Rpn_Redraw();
    }
    
}


void Rpn_Reset(void)
{
    EntryMode = RPN_NO_INPUT;
    if (MainStack == NULL)
        MainStack = BigIntStack_ctor(99);
    Rpn_Redraw();
}


bool Rpn_SendKey(sk_key_t k)
{
    bool r;
    signed char x;
    unsigned int i;
    if (k == sk_Enter)
    {
        r = AcquireInput();
        Rpn_Redraw();
        return r;
    }
    if (BigIntStack_IsEmpty(MainStack))
        return false;
    switch (k)
    {
        case sk_Del:
            r = !!BigIntStack_Pop(MainStack, NULL);
            Rpn_Redraw();
            return r;
        case sk_Ins:
            if (EntryMode == RPN_NO_INPUT)
                if (BigIntStack_GetSize(MainStack) > 0)
                    BigIntStack_Push(MainStack, BigIntStack_Peek(MainStack));
                else
                    return false;
            else
                return false;
            break;
        case sk_Chs:
            BigIntNegate(BigIntStack_Peek(MainStack));
            break;
        case sk_DecPnt:
            BigIntNot(BigIntStack_Peek(MainStack));
            break;
        case sk_Comma:
            BigIntStack_Exchange(MainStack);
            break;
        case sk_LParen:
            BigIntStack_RotateDown(MainStack);
            break;
        case sk_RParen:
            BigIntStack_RotateUp(MainStack);
            break;
        case sk_Add:
        case sk_Sub:
        case sk_Mul:
        case sk_Div:
        case sk_Square:
        case sk_Log:
        case sk_Ln:
        case sk_Power:
        case sk_Tan:
        case sk_2nd_Power:
        case sk_2nd_Tan:
        case sk_2nd_Div:
        case sk_2nd_Mul:
            AcquireInput();
            if (!EnsureBinaryOp())
                return false;
            switch (k)
            {
                case sk_Add:
                    BigIntAdd(topOfStack, &Temp1);
                    break;
                case sk_Sub:
                    BigIntSubtract(topOfStack, &Temp1);
                    break;
                case sk_Mul:
                    BigIntMultiply(BigIntStack_Pop(MainStack, NULL), &Temp1, &Temp2);
                    BigIntStack_Push(MainStack, &Temp2);
                    break;
                case sk_Div:
                    BigIntDivide(BigIntStack_Pop(MainStack, NULL), &Temp1, &Temp2, &Temp3);
                    BigIntStack_Push(MainStack, &Temp2);
                    break;
                case sk_2nd_Div:
                    BigIntDivide(BigIntStack_Pop(MainStack, NULL), &Temp1, &Temp2, &Temp3);
                    BigIntStack_Push(MainStack, &Temp3);
                    break;
                case sk_2nd_Mul:
                    BigIntDivide(BigIntStack_Pop(MainStack, NULL), &Temp1, &Temp2, &Temp3);
                    BigIntStack_Push(MainStack, &Temp3);
                    BigIntStack_Push(MainStack, &Temp2);
                    break;
                case sk_Square:
                    i = BigIntToNativeInt(&Temp1);
                    if (i > BIG_INT_SIZE * 8)
                        BigIntSetToZero(topOfStack);
                    else
                        while (i --> 0)
                            BigIntShiftLeft(topOfStack);
                    break;
                case sk_Log:
                    i = BigIntToNativeInt(&Temp1);
                    if (i > BIG_INT_SIZE * 8)
                        BigIntSetToZero(topOfStack);
                    else
                        while (i --> 0)
                            BigIntShiftRight(topOfStack);
                    break;
                case sk_Ln:
                    i = BigIntToNativeInt(&Temp1);
                    if (i > BIG_INT_SIZE * 8)
                        BigIntSetToZero(topOfStack);
                    else
                        while (i --> 0)
                            BigIntSignedShiftRight(topOfStack);
                    break;
                case sk_Power:
                    BigIntAnd(topOfStack, &Temp1);
                    break;
                case sk_Tan:
                    BigIntOr(topOfStack, &Temp1);
                    break;
                case sk_2nd_Power:
                    BigIntNand(topOfStack, &Temp1);
                    break;
                case sk_2nd_Tan:
                    BigIntOr(topOfStack, &Temp1);
                    BigIntNot(topOfStack);
                    break;
            }
            break;
        default:
            return false;
    }
    Rpn_Redraw();
    return true;
}