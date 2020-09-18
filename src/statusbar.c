#include <fontlibc.h>
#include <tice.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "statusbar.h"
#include "settings.h"
#include "rpnui.h"
#include "printbigint.h"
#include "ez80.h"
#include "gfx/gfx.h"
#include "ui.h"

CharTextWindow_t StatusBar_Window =
{
    0, 0,
    LCD_WIDTH, 0,
    0, 0
};


static uint16_t batteryPips[7];

static uint8_t batteryLevel;
static uint8_t batteryCharging;
static unsigned int batteryTimer = 0x0FFF;

static void drawBatteryIcon(void)
{
    if (batteryCharging)
        lcd_Palette[COLOR_BATTERY_OUTLINE] = lcd_Palette[COLOR_BATTERY_CHARGING];
    else if (batteryLevel < 2)
        lcd_Palette[COLOR_BATTERY_OUTLINE] = lcd_Palette[COLOR_BATTERY_DISCHARGED];
    else
        lcd_Palette[COLOR_BATTERY_OUTLINE] = lcd_Palette[COLOR_BATTERY_NORMAL];
    memcpy(&lcd_Palette[COLOR_BATTERY_LEVEL_1], &batteryPips[batteryLevel], sizeof(uint16_t) * 3);
    gfx_TransparentSprite_NoClip(battery_icon, StatusBar_Window.X + StatusBar_Window.Width - battery_icon_width - 1, StatusBar_Window.Y + 1);
}

static bool updateBatteryLevel(void)
{
    unsigned char level = batteryLevel;
    unsigned char charging = batteryCharging;
    batteryLevel = boot_GetBatteryStatus();
    batteryCharging = boot_IsCharging();
    batteryTimer = RtcTimer_Start(10);
    return level != batteryLevel || charging != batteryCharging;
}

void StatusBar_UpdateBatteryLevel(void)
{
    if (RtcTimer_Expired(batteryTimer))
        if (updateBatteryLevel())
            drawBatteryIcon();
}


static CharTextWindow_t oldWindow;

void StatusBar_Draw(void)
{
    char* s;
    if (!Settings.StatusBarEnabled)
        return;
    Style_SaveTextWindow(&oldWindow);
    Style_RestoreTextWindow(&StatusBar_Window);
    fontlib_SetForegroundColor(COLOR_STATUS_BAR_FOREGROUND);
    fontlib_SetBackgroundColor(COLOR_STATUS_BAR_BACKGROUND);
    fontlib_HomeUp();
    Style_SetSmallFontPropBold();
    fontlib_SetLineSpacing(1, 1);
    //fontlib_DrawString(" PROGRAMMER'S CALCULATOR");
    fontlib_DrawString(" Programmer's Calculator");
    fontlib_ClearEOL();
    Style_SetSmallFontPropBold();
    fontlib_SetLineSpacing(1, 1);
    fontlib_SetCursorPosition(150, 0);
    fontlib_DrawString(GetDisplayBitsName(Settings.DisplayBits));
    fontlib_DrawString(" bits");
    fontlib_SetCursorPosition(220, 0);
    fontlib_DrawString(GetBaseShortCapsName(Settings.PrimaryBase));
    fontlib_SetCursorPosition(260, 0);
    if (Settings.SecondaryBase != NO_BASE)
    {
        fontlib_DrawString("(");
        fontlib_DrawString(GetBaseShortCapsName(Settings.SecondaryBase));
        fontlib_DrawGlyph(')');
    }
    fontlib_SetForegroundColor(COLOR_FOREGROUND);
    fontlib_SetBackgroundColor(COLOR_BACKGROUND);
    Style_RestoreTextWindow(&oldWindow);
    drawBatteryIcon();
}


void StatusBar_Enable(void)
{
    unsigned char i;
    uint16_t* p = &batteryPips[0];
    Settings.StatusBarEnabled = true;
    StatusBar_Window.Height = SMALL_FONT_HEIGHT + 2;
    Rpn_Window.Height -= SMALL_FONT_HEIGHT + 2;
    Rpn_Window.Y = SMALL_FONT_HEIGHT + 2;
    for (i = 0; i < 4; i++)
        *p++ = lcd_Palette[COLOR_BATTERY_EMPTY_PIP];
    for (i = 0; i < 3; i++)
        *p++ = lcd_Palette[COLOR_BATTERY_PIP];
    updateBatteryLevel();
}


void StatusBar_Disable(void)
{
    Settings.StatusBarEnabled = false;
    StatusBar_Window.Height = 0;
    Rpn_Window.Height += SMALL_FONT_HEIGHT + 2;
    Rpn_Window.Y = 0;
}


bool StatusBar_IsEnabled(void)
{
    return Settings.StatusBarEnabled;
}
