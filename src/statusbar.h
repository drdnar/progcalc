#ifndef STATUS_BAR_H
#define STATUS_BAR_H
#include <stdbool.h>
#include "style.h"

/**
 * Specifies the area where the status bar will appear.
 */
extern CharTextWindow_t StatusBar_Window;

/**
 * Fully redraws the status bar.
 */
void StatusBar_Draw(void);

/**
 * Turns on the status bar.
 */
void StatusBar_Enable(void);

/**
 * Turns off the status bar.
 */
void StatusBar_Disable(void);

/** 
 * Returns whether the status bar is active.
 * @return True if the status bar should be drawn.
 */
bool StatusBar_IsEnabled(void);

/**
 * Updates the battery level display if the battery has changed.
 * This internally uses the RTC to determine how often to poll the battery.
 */
void StatusBar_UpdateBatteryLevel(void);

/**
 * Checks if the calculator is currently charging.
 * @return Either 0 or a non-zero number (not necessarily only 0 or 1).
 */
#define boot_IsCharging ((unsigned char (*)(void))0x3CC)

#endif /* STATUS_BAR_H */
