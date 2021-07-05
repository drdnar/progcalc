#ifndef MISC_H
#define MISC_H

#include <tice.h>
#include "forms/widget.h"

#ifdef __cplusplus
extern "C" {
#endif

enum FontId
{
    FONT_LARGE_PROP,
    FONT_LARGE_MONO,
    FONT_SMALL_PROP,
    FONT_SMALL_PROP_BOLD,
    FONT_SMALL_PROP_ALIGNED
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
