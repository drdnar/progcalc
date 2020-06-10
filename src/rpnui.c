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

/**
 * Waiting for user to start typing.
 */
#define ENTRY_NO_INPUT 0
/**
 * User has started typing a number.
 */
#define ENTRY_INPUT 1
/**
 * User is scrolling around the stack.
 */
#define ENTRY_SCROLL 2

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


/**
 * Redraws the entire stack display.
 */
static void DrawStack(void)
{
    unsigned int index;
    Style_SetLargeFontProp();
    Style_RestoreTextWindow(&StackWindow);
    if (EntryMode == ENTRY_INPUT)
        GetBigInt_Redraw();
    if (EntryMode == ENTRY_NO_INPUT || EntryMode == ENTRY_INPUT)
        index = 0;
    else
        /* SCROLLING NOT YET IMPLEMENTED */
        index = 0;
    if (BigIntStack_GetSize(MainStack))
        do
            ;
        while (DrawStackEntry(index++));
    else
        fontlib_DrawString("(Stack is empty.)");
}


void Rpn_Main(void)
{
    EntryMode = ENTRY_NO_INPUT;
    if (MainStack == NULL)
        MainStack = BigIntStack_ctor(99);
    DrawStack();
    
        
}