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
 * Draws an item, and also erases the space to the left.
 */
static void DrawItem(BigInt_t* n, Base_t base)
{
    Coord_t home;
    unsigned int x1, x2;
    unsigned char y1;
    Style_SaveCursor(&home);
    x2 = Format_PrintInBase(n, base);
    gfx_SetColor(fontlib_GetBackgroundColor());
    gfx_FillRectangle_NoClip(home.x, home.y, x2 - home.x, fontlib_GetCursorY() - home.y);
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
    if (home.y + requiredHeight > fontlib_GetWindowYMin() + fontlib_GetWindowHeight())
        return false;
    DrawItem(BigIntStack_Get(MainStack, n, NULL), Settings.PrimaryBase);
    Style_SaveCursor(&nextLoc);
    Style_RestoreCursor(&home);
    fontlib_DrawUInt(n, 2);
    fontlib_DrawGlyph(':');
    Style_RestoreCursor(&nextLoc);
    return true;
}


/**
 * Redraws the number the user is currently entering.
 */
static void DrawInput(void)
{
    Coord_t cursorHome, nextLoc;
    fontlib_HomeUp();
    Style_SaveCursor(&cursorHome);
    DrawItem(&CurrentInput, Settings.PrimaryBase);
    Style_SaveCursor(&nextLoc);
    Style_RestoreCursor(&cursorHome);
    fontlib_DrawString("#");
    Style_RestoreCursor(&nextLoc);
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
        DrawInput();
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