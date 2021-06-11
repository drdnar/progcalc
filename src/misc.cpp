#include "misc.h"
#include <fontlibc.h>
#include "forms/textmanager.h"

using namespace Forms;

extern "C" const unsigned int ApdDimTime = 60;
extern "C" const unsigned int ApdQuitTime = 120;

/******************************************************************************/

bool Is83Premium(void)
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

/**
 * Magic class that ensures the special FONT_SMALL_PROP_ALIGNED gets initialized.
 */
class FontInitHook final : FontManager
{
    private:
        static FontInitHook instance;
        FontInitHook(void)
        {
            /* This is bad but I dislike the alternatives more. */
            FontDescriptor* const_hack = (FontDescriptor*)FontsList;
            /*unsigned char big_height = FontManager::GetFont(FONT_LARGE_PROP)->height;
            unsigned char small_height = FontManager::GetFont(FONT_SMALL_PROP)->height;
            unsigned char delta_h = big_height - small_height;
            const_hack[FONT_SMALL_PROP_ALIGNED].OverrideSpaceAbove = delta_h;
            const_hack[FONT_SMALL_PROP_ALIGNED].OverrideSpaceBelow = big_height - delta_h - small_height;
            FontManager::ReloadFonts();*/
        }
};

FontInitHook FontInitHook::instance;
