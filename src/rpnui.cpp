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
#include "misc.h"
#include "forms/textmanager.h"

#include <debug.h>

using namespace Forms;

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
static unsigned char InputMode;

/**
 * Main RPN stack.
 */
static BigIntStack_t* MainStack = NULL;

/**
 * Number currently being entered by user.
 */
static BigInt_t CurrentInput;

/*static BigInt_t Temp0;*/
static BigInt_t Temp1;
static BigInt_t Temp2;
static BigInt_t Temp3;

/**
 * Current index stack is scrolled to.
 */
static unsigned int ScrollIndex = 0;

/**
 * Default window for showing stack and user entry.
 */
CharTextWindow_t Rpn_Window =
{
    0, 0,
    LCD_WIDTH, LCD_HEIGHT,
    0, 0,
    FONT_LARGE_PROP
};

/**
 * Caches the height of a single stack entry.
 */
static unsigned char EntryHeight;


/**
 * Acquires the user's current input and flushes the input buffer.
 */
static bool AcquireInput(void)
{
    if (!GetBigInt_IsActive())
        return false;
    InputMode = RPN_NO_INPUT;
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
//    BigIntStack_Exchange(MainStack);
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
    CursorLoc nextLoc;
    /* Force promotion so there's no overflow issues. */
    signed int printY;
    fontlib_Home();
    CursorSaver oldLoc;
    printY = oldLoc.Location.y - EntryHeight;
    if (n >= BigIntStack_GetSize(MainStack) || printY < Rpn_Window.Y)
        return false;
    oldLoc.Location.y = (unsigned char)printY;
    oldLoc.Location.Restore();
    Format_PrintInBase(BigIntStack_Get(MainStack, n, NULL), Settings::GetPrimaryBase());
    nextLoc.Save();
    oldLoc.Location.Restore();
    fontlib_DrawUInt(n, 2);
    fontlib_DrawGlyph(':');
    if (Settings::GetSecondaryBase() != NO_BASE)
    {
        nextLoc.Restore();
        Format_PrintInBase(BigIntStack_Get(MainStack, n, NULL), Settings::GetSecondaryBase());
    }
    return true;
}


void Rpn_Redraw(void)
{
    CursorLoc upperLeft;
    unsigned int index;
    Style_RestoreTextWindow(&Rpn_Window);
/*    Style_SetLargeFontProp();*/
    fontlib_SetCursorPosition(Rpn_Window.X, Rpn_Window.Y + Rpn_Window.Height);
    /* Cache height of an entry. TODO: This might NOT actually be helpful to cache here. */
    EntryHeight = Format_GetNumberHeight(Settings::GetPrimaryBase()) + Format_GetNumberHeight(Settings::GetSecondaryBase());
    /*if (InputMode == RPN_INPUT) Shouldn't be necessary to mae this conditional, as the redraw routine already checks this? */
        GetBigInt_Redraw();
    upperLeft.Save();
    if (InputMode == RPN_NO_INPUT || InputMode == RPN_INPUT)
        index = 0;
    else
    {
        /* TODO: SCROLLING NOT YET IMPLEMENTED */
        index = ScrollIndex;
    }
    if (BigIntStack_GetSize(MainStack))
    {
        do
            ;
        while (DrawStackEntry(index++));
        upperLeft.Save();
    }
    else if (InputMode == RPN_NO_INPUT)
    {
        Style_SetLargeFontProp();
        upperLeft.y = fontlib_GetCursorY() - fontlib_GetCurrentFontHeight();
        fontlib_SetCursorPosition(Rpn_Window.X, upperLeft.y);
        fontlib_DrawString("(Stack is empty.)");
        fontlib_Newline();
    }
    /* Erase remaining portion of window. */
    gfx_SetColor(fontlib_GetBackgroundColor());
    gfx_FillRectangle_NoClip(Rpn_Window.X, Rpn_Window.Y, Rpn_Window.Width, upperLeft.y - Rpn_Window.Y);
}


void Rpn_SetInputMode(bool mode)
{
    if (mode)
    {
        if (InputMode == RPN_INPUT)
            return;
        /* else */
        /* Handle starting entry */
        InputMode = RPN_INPUT;
        Rpn_Redraw();
    }
    else
    {
        if (InputMode == RPN_NO_INPUT)
            return;
        /* else */
        /* Handle stopping entry */
        InputMode = RPN_NO_INPUT;
        Rpn_Redraw();
    }
    
}


bool Rpn_IsScrollingActive(void)
{
    return InputMode == RPN_SCROLL;
}


void Rpn_Reset(void)
{
    InputMode = RPN_NO_INPUT;
    if (MainStack == NULL)
        MainStack = BigIntStack_ctor(99);
    Rpn_Redraw();
}


/**
 * Scrolls to the almost-last entry depending on screen space available.
 */
static void scrollLast(void)
{
    unsigned int windowSize = Rpn_Window.Height;
    if (GetBigInt_IsActive())
        windowSize -= GetBigInt_Window.Height;
    ScrollIndex = BigIntStack_GetSize(MainStack) - (windowSize / EntryHeight);
    InputMode = RPN_SCROLL;
}


bool Rpn_SendKey(sk_key_t k)
{
    bool r;
    unsigned int i;
    if (k == sk_Enter && InputMode != RPN_SCROLL)
    {
        r = AcquireInput();
        Rpn_Redraw();
        return r;
    }
    if (BigIntStack_IsEmpty(MainStack))
        return false;
    if (Rpn_IsScrollingActive())
    {
        switch (k)
        {
            case sk_Up:
                if (ScrollIndex == 0)
                    return false;
                ScrollIndex--;
                break;
            case sk_Down:
                if (ScrollIndex == BigIntStack_GetSize(MainStack) - 1)
                    return false;
                ScrollIndex++;
                break;
            case sk_2nd_Down:
                scrollLast();
                break;
            case sk_Clear:
            case sk_2nd_Clear:
            case sk_2nd_Up:
            case sk_Del:
            case sk_Ins:
            case sk_Mode:
            case sk_Enter:
                ScrollIndex = 0;
                if (GetBigInt_IsActive())
                    InputMode = RPN_INPUT;
                else
                    InputMode = RPN_NO_INPUT;
                break;
            default:
                /* If scrolling is active, then don't allow other actions. */
                return false;
        }
        if (ScrollIndex == 0)
        {
            if (GetBigInt_IsActive())
                InputMode = RPN_INPUT;
            else
                InputMode = RPN_NO_INPUT;
        }
    }
    else
    {
        switch (k)
        {
            case sk_Del:
                r = !!BigIntStack_Pop(MainStack, NULL);
                Rpn_Redraw();
                return r;
            case sk_Ins:
                if (InputMode == RPN_NO_INPUT)
                    if (BigIntStack_GetSize(MainStack) > 0)
                        BigIntStack_Push(MainStack, BigIntStack_Peek(MainStack));
                    else
                        return false;
                else
                    return false;
                break;
            case sk_Down:
                if (BigIntStack_GetSize(MainStack) < 2)
                    return false;
                InputMode = RPN_SCROLL;
                ScrollIndex = 1;
                break;
            case sk_2nd_Down:
                if (BigIntStack_GetSize(MainStack) < 2)
                    return false;
                scrollLast();
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
                        BigIntXor(topOfStack, &Temp1);
                        break;
                }
                break;
            default:
                return false;
        }
    }
    Rpn_Redraw();
    return true;
}