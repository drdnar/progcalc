#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <tice.h>
#include <fontlibc.h>
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
unsigned char EntryMode;

/**
 * Main RPN stack.
 */
BigIntStack_t* MainStack = NULL;

/**
 * Number currently being entered by user.
 */
BigInt_t CurrentInput;

/**
 * Default window for showing stack and user entry.
 */
CharTextWindow_t StackWindow =
{
    0, 0,
    LCD_WIDTH, LCD_HEIGHT,
    0, 0
};


/**
 * Redraws the number the user is currently entering.
 */
static unsigned char DrawEntry(void)
{
    fontlib_HomeUp();
    fontlib_DrawString("# ");
    return Format_PrintInPrimaryBase(&CurrentInput);
}


/**
 * Redraws the stack display.
 */
static void DrawStack(void)
{
    Style_SetLargeFontProp();
    Style_RestoreTextWindow(&StackWindow);
    if (EntryMode == ENTRY_INPUT)
        DrawEntry();
    if (EntryMode == ENTRY_NO_INPUT || EntryMode == ENTRY_INPUT)
    {

    }
}


void Rpn_Main(void)
{
    EntryMode = ENTRY_NO_INPUT;
    if (MainStack == NULL)
        MainStack = BigIntStack_ctor(256);
    
    
        
}