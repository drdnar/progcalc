#ifndef FORMS_KEYBOARD_EVENT_SOURCE_H
#define FORMS_KEYBOARD_EVENT_SOURCE_H
#include "eventsource.h"
#include <graphx.h>

extern "C" const unsigned int ApdDimTime;
extern "C" const unsigned int ApdQuitTime;

namespace Forms
{

/**
 * Process keyboard events.
 * TODO: In the old C code, this also handled the 2nd indicator and APD.
 * The location of the 2nd indicator is still global state, which should be
 * changed.
 * APD timing should maybe be broken off into its own object.
 */
class KeyboardEventSource final : public EventSource
{
    public:
        WidgetMessage GetEvent(void);
        /**
         * Returns an object that the GUI subsystem can get events from.
         */
        static KeyboardEventSource& GetInstance(void) { return instance; }
        ~KeyboardEventSource();
        /**
         * Turns on display of 2nd/Alpha modifier indicator.
         */
        static void EnableIndicator(void);
        /**
         * Turns off display of 2nd/Alpha modifier indicator.
         */
        static void DisableIndicator(void);
        /**
         * Checks whether 2nd/Alpha modifier indicator will be shown.
         */
        static bool IndicatorEnabled(void) { return cursor_enabled; }
        /**
         * Sets the location of the 2nd/Alpha modifier indicator.
         */
        static void SetIndicatorLocation(x_t x, y_t y);
        /**
         * Gets the location of the 2nd/Alpha modifier indicator.
         */
        static x_t GetIndicatorX(void) { return cursor_x; }
        /**
         * Gets the location of the 2nd/Alpha modifier indicator.
         */
        static y_t GetIndicatorY(void) { return cursor_y; }
        /**
         * Enables special processing of 2nd key.
         */
        static void Enable2nd(void);
        /**
         * Disables special processing of 2nd key, so pressing 2nd returns
         * sk_2nd instead of acting as a dead key.
         */
        static void Disable2nd(void);
        /**
         * Checks whether special process of 2nd key is enabled.
         */
        static bool Is2ndEnabled(void) { return second_enabled; }
        /**
         * Clears away the 2nd indicator and state.
         */
        static void Clear2nd(void) { reset2nd(); }
        /**
         * Simulates pressing 2nd if and only if 2nd processing is enabled.
         */
        static void Force2nd(void) { set2nd(); }
        /**
         * Checks whether the 2nd modifier is currently active.
         */
        static bool Is2ndPressed(void) { return second; }
    private:
        unsigned int count = 0;
        /**
         * This is magic hook that ensures this gets initialized.
         */
        static KeyboardEventSource instance;
        /**
         * Configures how many seconds to wait before dimming the screen.
         */
        const unsigned int APD_DIM_TIME = 60;
        /**
         * Configures how many seconds to wait to quit after dimming the screen.
         */
        const unsigned int APD_QUIT_TIME = 120;
        /**
         * Assumed height of the indicator cursor.
         */
        const unsigned char INDICATOR_HEIGHT = 14;
        KeyboardEventSource(void);
        /*KeyboardEventSource(KeyboardEventSource const& x) = default;
        KeyboardEventSource(KeyboardEventSource&& x) = default;*/
        /**
         * Restarts the APD count-down.
         */
        static void restart_apd(void);
        /**
         * Checks the APD timer.  Ends the program if it has expired.
         */
        static void check_apd(void);
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
        static void show_cursor(void);
        /**
         * Internal routine that restores whatever the cursor overwrote.
         */
        static void unshow_cursor(void);
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
        static void set2nd(void);
        /**
         * Internally implements 2nd.
         */
        static void reset2nd(void);
        /**
         * Caches state of alpha modifier.
         */
        //static bool alpha = false;
        /**
         * Caches state of capital alpha modifier.
         */
        //static bool ALPHA = false;
        /**
         * Power-saving timer.
         */
        static unsigned int apdtimer;
        /**
         * True when display dimming is active.
         */
        static bool dimmed;
        /**
         * Caches whatever was under the 2nd/alpha indicator.
         */
        static gfx_sprite_t* under_indicator;
};


} /* namespace Forms */

#endif /* FORMS_KEYBOARD_EVENT_SOURCE_H */
