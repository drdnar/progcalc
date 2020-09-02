#ifndef RPN_UI_H
#define RPN_UI_H
#include <stdbool.h>
#include <tice.h>
#include "style.h"

/**
 * Specifies the screen space available for the RPN UI.
 */
extern CharTextWindow_t Rpn_Window;

/**
 * Handles a keypress, performing any actions required.
 * @param k Key code to handle
 * @return True if keypress was handled, false if not.
 */
bool Rpn_SendKey(sk_key_t k);

/**
 * Forces redraw of the stack and user input.
 */
void Rpn_Redraw(void);

/**
 * Resets/initializes RPN entry.
 */
void Rpn_Reset(void);

/**
 * Informs the RPN UI whether the user is currently entering a number.
 * If stack scrolling is active, then stack scrolling is exited.
 * @param mode True if the user is entering a number, false if not.
 */
void Rpn_SetInputMode(bool mode);


#endif /* RPN_UI_H */
