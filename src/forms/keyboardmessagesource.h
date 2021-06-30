#ifndef FORMS_KEYBOARD_EVENT_SOURCE_H
#define FORMS_KEYBOARD_EVENT_SOURCE_H
#include "messages.h"
#include <graphx.h>

extern "C" const unsigned int ApdDimTime;
extern "C" const unsigned int ApdQuitTime;

namespace Forms
{

/**
 * Processes keyboard events.
 * TODO: In the old C code, this also handled the 2nd indicator.
 * The location of the 2nd indicator is still global state, which should be
 * changed.
 */
class KeyboardEventSource final : public MessageSource
{
    public:
        Message GetMessage();
        /**
         * Returns an object that the GUI subsystem can get events from.
         */
        static KeyboardEventSource& GetInstance() { return instance; }
        ~KeyboardEventSource();
        /**
         * Turns on display of 2nd/Alpha modifier indicator.
         */
        static void EnableIndicator();
        /**
         * Turns off display of 2nd/Alpha modifier indicator.
         */
        static void DisableIndicator();
        /**
         * Checks whether 2nd/Alpha modifier indicator will be shown.
         */
        static bool IndicatorEnabled() { return cursor_enabled; }
        /**
         * Sets the location of the 2nd/Alpha modifier indicator.
         */
        static void SetIndicatorLocation(x_t x, y_t y);
        /**
         * Gets the location of the 2nd/Alpha modifier indicator.
         */
        static x_t GetIndicatorX() { return cursor_x; }
        /**
         * Gets the location of the 2nd/Alpha modifier indicator.
         */
        static y_t GetIndicatorY() { return cursor_y; }
        /**
         * Enables special processing of 2nd key.
         */
        static void Enable2nd();
        /**
         * Disables special processing of 2nd key, so pressing 2nd returns
         * sk_2nd instead of acting as a dead key.
         */
        static void Disable2nd();
        /**
         * Checks whether special process of 2nd key is enabled.
         */
        static bool Is2ndEnabled() { return second_enabled; }
        /**
         * Clears away the 2nd indicator and state.
         */
        static void Clear2nd() { reset2nd(); }
        /**
         * Simulates pressing 2nd if and only if 2nd processing is enabled.
         */
        static void Force2nd() { set2nd(); }
        /**
         * Checks whether the 2nd modifier is currently active.
         */
        static bool Is2ndPressed() { return second; }
    private:
        /**
         * This is magic hook that ensures this gets initialized.
         */
        static KeyboardEventSource instance;
        /**
         * Assumed height of the indicator cursor.
         */
        const unsigned char INDICATOR_HEIGHT = 14;
        KeyboardEventSource();
        /*KeyboardEventSource(KeyboardEventSource const& x) = default;
        KeyboardEventSource(KeyboardEventSource&& x) = default;*/
        /**
         * Controls whether a cursor indicator will be shown.
         */
        static bool cursor_enabled;
        /**
         * Remember whether a cursor is being shown.
         */
        static bool cursor_shown;
        /**
         * Cursor location.
         */
        static x_t cursor_x;
        /**
         * Cursor location.
         */
        static y_t cursor_y;
        /**
         * Internal routine that draws the cursor.
         */
        static void show_cursor();
        /**
         * Internal routine that restores whatever the cursor overwrote.
         */
        static void unshow_cursor();
        /**
         * Controls whether 2nd acts as a dead key.
         */
        static bool second_enabled;
        /**
         * Caches state of 2nd modifier.
         */
        static bool second;
        /**
         * Internally implements 2nd.
         */
        static void set2nd();
        /**
         * Internally implements 2nd.
         */
        static void reset2nd();
        /**
         * Caches state of alpha modifier.
         */
        //static bool alpha = false;
        /**
         * Caches state of capital alpha modifier.
         */
        //static bool ALPHA = false;
        /**
         * Caches whatever was under the 2nd/alpha indicator.
         */
        static gfx_sprite_t* under_indicator;
};


} /* namespace Forms */

#endif /* FORMS_KEYBOARD_EVENT_SOURCE_H */
