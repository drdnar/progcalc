#ifndef EZ80_H
#define EZ80_H

#ifdef __cplusplus
extern "C" {
#endif


/**
 * Checks if routine is NULL.  If not, runs routine.
 * @warning CONTAINS SMC, cannon run from flash
 */
//unsigned char CallIfNotNull(void (*)(...), ...);

/**
 * Checks if routine is NULL.  If not, runs routine.
 * @warning CONTAINS SMC, cannon run from flash
 * @return If routine is NULL, returns 0.  Otherwise, returns what the routine returned.
 */
//unsigned char CallIfNotNull8(unsigned char (*)(...), ...);

/**
 * Checks if routine is NULL.  If not, runs routine.
 * @warning CONTAINS SMC, cannon run from flash
 * @return If routine is NULL, returns 0.  Otherwise, returns what the routine returned.
 */
//unsigned int CallIfNotNull24(unsigned int (*)(...), ...);

/**
 * Switches the LCD to dimmer power-saving mode.
 */
void Lcd_Dim(void);

/**
 * Restores the LCD to the user's configured brightness.
 */
void Lcd_Bright(void);

/**
 * Requests the calculator turn off when the program ends.
 * If run from a shell, this probably won't do anything, which is intentional
 * behavior.
 */
void TurnOffOnExit(void);

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
 * Wraps GetCSC, but also implements a sort of ERR: BREAK handler.
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

#ifdef __cplusplus
}
#endif

#endif
