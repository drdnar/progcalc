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
static void AcquireInput(void)
{
    if (!GetBigInt_IsActive())
        return;
    EntryMode = RPN_NO_INPUT;
    GetBigInt(&CurrentInput);
    GetBigInt_Reset();
    BigIntStack_Push(MainStack, &CurrentInput);
}


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
    switch (k)
    {
        case sk_Del:
            r = !!BigIntStack_Pop(MainStack, NULL);
            Rpn_Redraw();
            return r;
        case sk_Enter:
            /* Duplicate top entry if entry not active */
            AcquireInput();
            Rpn_Redraw();
            return true;
        case sk_Add:
            AcquireInput();
            if (!EnsureBinaryOp())
                return false;
            BigIntAdd(BigIntStack_Peek(MainStack), &Temp1);
            Rpn_Redraw();
            return true;
        case sk_Sub:
            AcquireInput();
            if (!EnsureBinaryOp())
                return false;
            BigIntSubtract(BigIntStack_Peek(MainStack), &Temp1);
            Rpn_Redraw();
            return true;
        case sk_Mul:
            AcquireInput();
            if (!EnsureBinaryOp())
                return false;
            BigIntMultiply(BigIntStack_Pop(MainStack, NULL), &Temp1, &Temp2);
            BigIntStack_Push(MainStack, &Temp2);
            Rpn_Redraw();
            return true;
        case sk_Div:
            AcquireInput();
            if (!EnsureBinaryOp())
                return false;
            BigIntDivide(BigIntStack_Pop(MainStack, NULL), &Temp1, &Temp2, &Temp3);
            BigIntStack_Push(MainStack, &Temp2);
            Rpn_Redraw();
            return true;
    }
    return false;
}