#ifndef STYLE_H
#define STYLE_H

#include <tice.h>
#include <graphx.h>

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
 * Color of highlighted backgrounds.
 */
#define COLOR_HIGHLIGHT 3
/**
 * Transparency color
 */
#define COLOR_TRANSPARENT 13
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
 * Main text background color.
 *
enum { BgColor = 7 };
/**
 * Main text color.
 *
enum { FgColor = 0 };
/**
 * Color of leading zeros.
 *
enum { ZeroColor = 2 };
/**
 * Background color of the status bar.
 *
enum { StatusBarBgColor = 1 };
/**
 * Foreground color of the status bar.
 *
enum { StatusBarFgColor = 7 };
/**
 * Color of highlighted backgrounds.
 *
enum { HighlightColor = 8 };

enum { BatteryOutlineColor = 3 };
*/


/**
 * Defines a coordinate.
 */
typedef struct
{
    unsigned int x;
    unsigned char y;
} Coord_t;

/**
 * For GetKey, controls where the cursor will show up when enabled.
 */
extern Coord_t CursorLocation;

#define CHAR_WIDTH 8
#define CHAR_HEIGHT 14

#define BIG_FONT_HEIGHT 14
#define SMALL_FONT_HEIGHT 10

#define GROUP_DELIMITER ' '
/**
 * Caches the width of a space in the large variable-width font.
 */
extern unsigned int GroupDelimiterWidth;

/**
 * Wraps a text window into a single type.
 */
typedef struct
{
    /* I wrote some code that depends on this mirroring the structure of Coord_t
     * so changing this struct may require other refactoring. */
    unsigned int X;
    unsigned char Y;
    unsigned int Width;
    unsigned char Height;
    unsigned int CursorX;
    unsigned char CursorY;
} CharTextWindow_t;

/**
 * Displays an error message on the homescreen, and then exits.
 * @param msg String to display.
 */
void ShowErrorAndExit(char* msg);

/**
 * Prints a string, centering it in the current text window.
 */
void Style_PrintCentered(const char* string);

/**
 * Prints a string right-aligned.
 */
void Style_PrintRight(const char* string);

/**
 * Initializes stuff related to graphics and layout.
 */
void Style_Initialize(void);

/**
 * Uninitializes stuff related to graphics and layout.
 */
void Style_Finalize(void);

/**
 * Saves the current cursor.
 * @param target Pointer to location to save cursor location information.
 */
void Style_SaveCursor(Coord_t* target);

/**
 * Restores the cursor location.
 * @param target Pointer to cursor location information.
 */
void Style_RestoreCursor(Coord_t* coord);

/**
 * Saves the current text window to a variable.
 * @param saveTarget Location to write text window properties
 */
void Style_SaveTextWindow(CharTextWindow_t* saveTarget);

/**
 * Restores a previously saved text window.
 * @param window Pointer to text window descriptor
 */
void Style_RestoreTextWindow(CharTextWindow_t* window);

/**
 * Changes the current font.
 */
void Style_SetLargeFontMono(void);

/**
 * Changes the current font.
 */
void Style_SetLargeFontProp(void);

/**
 * Changes the current font.
 */
void Style_SetSmallFontProp(void);

/**
 * Changes the current font.
 */
void Style_SetSmallFontPropBold(void);

/**
 * Changes the current font and ensures baseline alignment with the large font.
 */
void Style_SetSmallFontPropAligned(void);

#endif
