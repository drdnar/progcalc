#include "misc.h"
#include <fontlibc.h>
#include "forms/textmanager.h"
#include "forms/widget.h"
#include "forms/gui.h"
#include "forms/style.h"
#include "style.h"

using namespace Forms;

extern "C" const unsigned int ApdDimTime = 60;
extern "C" const unsigned int ApdQuitTime = 120;

/******************************************************************************/

bool Is83Premium()
{
    return os_GetSystemInfo()->hardwareType;
}


/******************************************************************************/

const FontDescriptor (&Forms::FontsList)[] =
{
    {"DrSans", 14, 14, FONTLIB_BOLD, FONTLIB_BOLD, 0, 0, (unsigned char)-1, (unsigned char)-1},
    {"DrMono", 14, 14, FONTLIB_BOLD, FONTLIB_BOLD, 0, 0, (unsigned char)-1, (unsigned char)-1},
    {"DrSans", 10, 10, FONTLIB_NORMAL, FONTLIB_NORMAL, 0, 0, (unsigned char)-1, (unsigned char)-1},
    {"DrSans", 10, 10, FONTLIB_BOLD, FONTLIB_BOLD, 0, 0, (unsigned char)-1, (unsigned char)-1},
    {"DrSans", 10, 10, FONTLIB_NORMAL, FONTLIB_NORMAL, 0, 0, (unsigned char)-1, (unsigned char)-1},
};

extern const unsigned char Forms::FontsCount = 5;


/******************************************************************************/

static Style gui_style
{
    FONT_LARGE_PROP,
    COLOR_FOREGROUND,
    COLOR_BACKGROUND,
    COLOR_HIGHLIGHT,
    COLOR_SHADOW
};

Style* const Forms::GUI_DefaultStyle = &gui_style;
