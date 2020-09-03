#ifndef INPUT_BIG_INT_H
#define INPUT_BIG_INT_H
#include <stdbool.h>
#include <tice.h>
#include "bigint.h"
#include "style.h"

/**
 * Caches the correct window settings for the input routines.
 */
extern CharTextWindow_t GetBigInt_Window;

/**
 * Handles a keypress, performing any actions required.
 * @param k Key code to handle
 * @return True if keypress was handled, false if not.
 */
bool GetBigInt_SendKey(sk_key_t k);

/**
 * Returns true if input is currently active.
 */
bool GetBigInt_IsActive(void);

/**
 * Resets input routines to no-input status.
 */
void GetBigInt_Reset(void);

/**
 * Adjusts the positioning logic to new input settings.
 */
void GetBigInt_Reposition(void);

/**
 * Redraws the user's current entry.
 * May draw nothing.
 * @note Assumes the current text window is set up properly.
 */
void GetBigInt_Redraw(void);

/**
 * Writes the user's current entry to a given variable.
 * @param Pointer to variable to receive entry.
 */
void GetBigInt(BigInt_t* n);

#endif /* INPUT_BIG_INT_H */
