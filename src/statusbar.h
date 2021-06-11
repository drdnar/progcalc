#ifndef STATUS_BAR_H
#define STATUS_BAR_H
#include <stdbool.h>
#include "style.h"
#include "forms/widget.h"
#include "forms/textmanager.h"
#include "misc.h"

namespace Forms
{

class StatusBar final : public Widget
{
    public:
        StatusBar(void);
        Status MoveTo(x_t x, y_t y);
        Status Paint(void);
        WidgetMessage SendInput(WidgetMessage message);
        Status Hide(void);
        Status Show(void);
        /**
         * Requests that the StatusBar consider updating the battery level.
         * This will only actually check for a battery level change every few
         * seconds.
         */
        void UpdateBatteryLevel(void);
    private:
        /**
         * Handles physical painting of the battery icon, as well as making sure
         * the level displayed matches the latest battery level measurement.
         */
        void _draw_battery_icon(void);
        /**
         * Measures the battery level and caches the result.
         */
        bool _update_battery_level(void);
        /**
         * Cached measurement of the battery's charge level.
         */
        static unsigned char battery_level;
        /**
         * Cached test of whether the battery is charging.
         */
        static bool battery_charging;
        /**
         * Keeps track of when the battery level should next be checked.
         */
        static unsigned int battery_timer;
        /**
         * The status bar's location is fixed.
         */
        const TextWindow window =
        {
            0, 0,
            LCD_WIDTH, 0,
            0, 0,
            FONT_SMALL_PROP_BOLD
        };
        static uint16_t battery_pips[7];
};


extern "C" {

/**
 * Checks if the calculator is currently charging.
 * @return Either 0 or a non-zero number (not necessarily only 0 or 1).
 */
#define boot_IsCharging ((unsigned char (*)(void))0x3CC)

} /* extern "C" */

} /* namespace Forms */

#endif /* STATUS_BAR_H */
