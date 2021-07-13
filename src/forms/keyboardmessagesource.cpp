#include "keyboardmessagesource.h"
#include <fontlibc.h>
#include <tice.h>
#include <stdlib.h>
#include <graphx.h>
#include "ez80.h"
#include "calc1252.h"
#include "textmanager.h"
#include "misc.h"

using namespace Forms;

KeyboardEventSource KeyboardEventSource::instance;
bool KeyboardEventSource::cursor_enabled = false;
bool KeyboardEventSource::cursor_shown = false;
x_t KeyboardEventSource::cursor_x = 0;
y_t KeyboardEventSource::cursor_y = 0;
bool KeyboardEventSource::enable_modifiers = true;
bool KeyboardEventSource::second_enabled = false;
bool KeyboardEventSource::second = 0;
//bool KeyboardEventSource::alpha = false;
//bool KeyboardEventSource::ALPHA = false;
gfx_sprite_t* KeyboardEventSource::under_indicator = nullptr;



KeyboardEventSource::KeyboardEventSource()
 : MessageSink(SINK_PRIORITY_MINIMAL)
{
    fontlib_font_t* font = fontlib_GetFontByStyle("DrSans", 14, 14, FONTLIB_BOLD, FONTLIB_BOLD, 0, 0);
    if (font)
    {
        fontlib_SetFont(font, (fontlib_load_options_t)0);
        under_indicator = gfx_MallocSprite(fontlib_GetGlyphWidth(CALC1252_CURSOR_2ND_CHAR), INDICATOR_HEIGHT);
    }
    /* Otherwise, initialization will fail later when trying to load fonts. */
    MessageLoop::RegisterSynchronousMessageSource(*this);
    MessageLoop::RegisterMessageSink(*this);
}


KeyboardEventSource::~KeyboardEventSource()
{
    free(under_indicator);
}


Message KeyboardEventSource::GetMessage()
{
    sk_key_t key = GetCSC_Breakable();
    if (key)
    {
        ClearOnKeyPressed();
        return { .Id = MESSAGE_RAW_KEY, .ExtendedCode = key };
    }
    return { .Id = MESSAGE_NONE, .ExtendedCode = MESSAGE_NONE };
}


/**
 * Table mapping keys to GUI key events.
 */
static MapTable GuiKeyMappings[] = 
{
    { sk_Down, GUI_KEY_DOWN },
    { sk_Left, GUI_KEY_LEFT },
    { sk_Right, GUI_KEY_RIGHT },
    { sk_Up, GUI_KEY_UP },
    { sk_Down | sk_2nd, GUI_KEY_PAGE_DOWN },
    { sk_Left | sk_2nd, GUI_KEY_HOME },
    { sk_Right | sk_2nd, GUI_KEY_END },
    { sk_Up | sk_2nd, GUI_KEY_PAGE_UP },
    { sk_Quit, GUI_KEY_EXIT },
    { sk_Clear, GUI_KEY_EXIT },
    { sk_Enter, GUI_KEY_ENTER },
    //{ sk_, GUI_KEY_ },  
};
static const size_t GuiKeyMappingsCount = sizeof(GuiKeyMappings) / sizeof(GuiKeyMappings[0]);


bool KeyboardEventSource::SendMessage(Message& message)
{
    unsigned int key = message.ExtendedCode;
    switch (message.Id)
    {
        case MESSAGE_RAW_KEY:
            // Process a raw key into an extended key.
            if (enable_modifiers)
            {
                if (second_enabled)
                {
                    if (key == sk_2nd)
                    {
                        if (!second)
                            set2nd();
                        else
                            reset2nd();
                        return false;
                    }
                    if (second)
                    {
                        key |= sk_2nd_Modifier;
                        reset2nd();
                    }
                }
            }
            MessageLoop::EnqueueMessage({ .Id = MESSAGE_KEY, .ExtendedCode = (MessageCode)key });
            return true;
        case MESSAGE_KEY:
            // Process a key into a GUI action.
            if (GuiKeyMappings->Map(GuiKeyMappingsCount, key))
            {
                MessageLoop::EnqueueMessage({ .Id = MESSAGE_GUI_KEY, .ExtendedCode = (MessageCode)key });
                return true;
            }
            return false;
        default:
            return false;
    }
}


void KeyboardEventSource::EnableIndicator()
{
    cursor_enabled = true;
    if (second)
        show_cursor();
}


void KeyboardEventSource::DisableIndicator()
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


void KeyboardEventSource::show_cursor()
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


void KeyboardEventSource::unshow_cursor()
{
    if (!cursor_shown)
        return;
    gfx_Sprite_NoClip(under_indicator, cursor_x, cursor_y);
    cursor_shown = false;
}


void KeyboardEventSource::EnableModifiers()
{
    enable_modifiers = true;
}


void KeyboardEventSource::DisableModifiers()
{
    reset2nd();
    enable_modifiers = false;
}


void KeyboardEventSource::Enable2nd()
{
    second_enabled = true;
}


void KeyboardEventSource::Disable2nd()
{
    reset2nd();
    second_enabled = false;
}


void KeyboardEventSource::set2nd()
{
    if (!second_enabled)
        return;
    show_cursor();
    second = true;
}


void KeyboardEventSource::reset2nd()
{
    if (!second_enabled)
        return;
    unshow_cursor();
    second = false;
}
