#ifndef EZ80_H
#define EZ80_H

/**
 * Switches the LCD to dimmer power-saving mode.
 */
void Lcd_Dim(void);

/**
 * Restores the LCD to the user's configured brightness.
 */
void Lcd_Bright(void);

/**
 * Returns the number of seconds elapsed since the start of the hour.
 * Used by APD routines.
 */
unsigned int GetRtcSeconds(void);

/**
 * Starts a timer, to expire after a given number of seconds have elapsed.
 * @param duration Number of seconds later for timer to expire
 * @return Check value to pass to RtcTimer_Expired
 */
#define RtcTimer_Start(duration) (GetRtcSeconds() + duration)

/**
 * Checks if a timer has expired, including a bit of wiggle room.
 * @return True if timer has expired, false if not.
 */
unsigned char RtcTimer_Expired(unsigned int);

/**
 * Wraps GetCSC with two differences:
 *  - Assumes you're using this in an input loop, so it does EI \ HALT to save a tiny bit of power.
 *  - If the ON key has been pressed and the user then presses CLEAR, aborts the program immediately.
 */
unsigned char GetCSC_Breakable(void);

/**
 * Returns 1 if the ON key has been pressed since the last time the
 * ON-key-pressed flag was checked.
 * Returns 0 if the ON key has not been pressed.
 */
bool CheckIfOnKeyPressed(void);

/**
 * Resets the ON-key-pressed flag.
 */
void ClearOnKeyPressed(void);

#endif
