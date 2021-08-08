#ifndef MISC_H
#define MISC_H

#include <tice.h>
#include "forms/widget.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef uint8_t Color_t;

/**
 * Main text color.
 */
#define COLOR_FOREGROUND 0
/**
 * Main text background color.
 */
#define COLOR_BACKGROUND 12
/**
 * Drop shadow color.
 */
#define COLOR_SHADOW 13
/**
 * Color of highlighted backgrounds.
 */
#define COLOR_HIGHLIGHT 3
/**
 * Transparency color
 */
#define COLOR_TRANSPARENT 14
/**
 * Color of leading zeros.
 */
#define COLOR_ZERO 2
/**
 * Background color of the status bar.
 */
#define COLOR_STATUS_BAR_BACKGROUND 1
/**
 * Foreground color of the status bar.
 */
#define COLOR_STATUS_BAR_FOREGROUND COLOR_BACKGROUND
/**
 * Battery outline index
 */
#define COLOR_BATTERY_OUTLINE 4
/**
 * Color of an empty battery level pip
 */
#define COLOR_BATTERY_EMPTY_PIP COLOR_BACKGROUND
/**
 * Battery level 1
 */
#define COLOR_BATTERY_LEVEL_1 5
/**
 * Battery level 2
 */
#define COLOR_BATTERY_LEVEL_2 6
/**
 * Battery level 3
 */
#define COLOR_BATTERY_LEVEL_3 7
/**
 * Battery level pip
 */
#define COLOR_BATTERY_PIP 8
/**
 * Normal battery outline color
 */
#define COLOR_BATTERY_NORMAL 9
/**
 * Charging battery color
 */
#define COLOR_BATTERY_CHARGING 10
/**
 * Discharged battery color
 */
#define COLOR_BATTERY_DISCHARGED 11


/**
 * List of user font IDs
 */
enum FontId
{
    FONT_LARGE_PROP,
    FONT_LARGE_MONO,
    FONT_SMALL_PROP,
    FONT_SMALL_PROP_BOLD
};


enum RPN_ID : uint8_t
{
    WIDGET_ID_RPN_UI = Forms::ID::WIDGET_ID_User
};

 
/**
 * Exits the program, cleaning up stuff.
 */
void ExitClean(void); /* I don't really have a better idea where to put this declaration. */


/**
 * Checks whether the device is an 83 Premium CE with the different key layout.
 * @return Hopefully returns true if the device is an 83 Premium CE.
 */
bool Is83Premium();


#ifdef __cplusplus
}
#endif

#endif /* MISC_H */
