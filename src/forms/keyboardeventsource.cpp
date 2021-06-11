#include "keyboardeventsource.h"
#include <fontlibc.h>
#include <tice.h>
#include <stdlib.h>
#include <graphx.h>
#include "../ez80.h"
#include "calc1252.h"
#include "textmanager.h"
#include "misc.h"

using namespace Forms;

/**
 * Exits the program, cleaning up stuff.
 */
extern "C" void ExitClean(void); /* I don't really have a better idea where to put this declaration. */


KeyboardEventSource KeyboardEventSource::instance;
bool KeyboardEventSource::cursor_enabled = false;
bool KeyboardEventSource::cursor_shown = false;
x_t KeyboardEventSource::cursor_x = 0;
y_t KeyboardEventSource::cursor_y = 0;
bool KeyboardEventSource::second_enabled = false;
bool KeyboardEventSource::second = 0;
//bool KeyboardEventSource::alpha = false;
//bool KeyboardEventSource::ALPHA = false;
unsigned int KeyboardEventSource::apdtimer = 0;
bool KeyboardEventSource::dimmed = false;
gfx_sprite_t* KeyboardEventSource::under_indicator = nullptr;



KeyboardEventSource::KeyboardEventSource(void)
{
    fontlib_font_t* font = fontlib_GetFontByStyle("DrSans", 14, 14, FONTLIB_BOLD, FONTLIB_BOLD, 0, 0);
    if (font)
    {
        fontlib_SetFont(font, (fontlib_load_options_t)0);
        under_indicator = gfx_MallocSprite(fontlib_GetGlyphWidth(CALC1252_CURSOR_2ND_CHAR), INDICATOR_HEIGHT);
    }
    /* Otherwise, initialization will fail later when trying to load fonts. */
    restart_apd();
}


KeyboardEventSource::~KeyboardEventSource()
{
    free(under_indicator);
}


void KeyboardEventSource::EnableIndicator(void)
{
    cursor_enabled = true;
    if (second)
        show_cursor();
}


void KeyboardEventSource::DisableIndicator(void)
{
    unshow_cursor();
    cursor_enabled = false;
}


void KeyboardEventSource::SetIndicatorLocation(x_t x, y_t y)
{
    bool cursor_on = cursor_shown;
    unshow_cursor();
    cursor_x = x;
    cursor_y = y;
    if (cursor_on)
        show_cursor();
}


void KeyboardEventSource::show_cursor(void)
{
    if (!cursor_enabled)
        return;
    if (cursor_shown)
        return;
    CursorSaver savedCursor;
    gfx_GetSprite(under_indicator, cursor_x, cursor_y);
    FontManager::SetFont(FONT_LARGE_PROP);
    fontlib_SetCursorPosition(cursor_x, cursor_y);
    /* Since the battery icon is updated by changing its palette entries,
     * there's no need to worry about the 2nd indicator being overwritten
     * when the battery icon is updated.*/
    fontlib_DrawGlyph(CALC1252_CURSOR_2ND_CHAR);
    cursor_shown = true;
}


void KeyboardEventSource::unshow_cursor(void)
{
    if (!cursor_shown)
        return;
    gfx_Sprite_NoClip(under_indicator, cursor_x, cursor_y);
    cursor_shown = false;
}


WidgetMessage KeyboardEventSource::GetEvent(void)
{
    sk_key_t key = GetCSC_Breakable();
    if (key)
    {
        ClearOnKeyPressed();
        restart_apd();
        if (second_enabled)
        {
            if (key == sk_2nd)
            {
                if (!second)
                    set2nd();
                else
                    reset2nd();
                return 0;
            }
            else if (second)
            {
                key |= sk_2nd_Modifier;
                reset2nd();
            }
        }
    }
    else
        check_apd();
    return key;
}


void KeyboardEventSource::Enable2nd(void)
{
    second_enabled = true;
}


void KeyboardEventSource::Disable2nd(void)
{
    reset2nd();
    second_enabled = false;
}


void KeyboardEventSource::set2nd(void)
{
    if (!second_enabled)
        return;
    show_cursor();
    second = true;
}


void KeyboardEventSource::reset2nd(void)
{
    if (!second_enabled)
        return;
    unshow_cursor();
    second = false;
}


void KeyboardEventSource::restart_apd(void)
{
    if (dimmed)
    {
        Lcd_Bright();
        dimmed = false;
    }
    apdtimer = GetRtcSeconds() + ApdDimTime;
}


void KeyboardEventSource::check_apd(void)
{
    if (RtcTimer_Expired(apdtimer))
    {
        if (!dimmed)
        {
            dimmed = true;
            Lcd_Dim();
            apdtimer = GetRtcSeconds() + ApdQuitTime;
        }
        else
            ExitClean();
    }
}
