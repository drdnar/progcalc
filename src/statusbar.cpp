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
#include "misc.h"
#include "forms/widget.h"
#include "forms/textmanager.h"
#include "forms/ignorewarning.h"

using namespace Forms;


unsigned char StatusBar::battery_level;

bool StatusBar::battery_charging;

unsigned int StatusBar::battery_timer;

uint16_t StatusBar::battery_pips[7];

StatusBar StatusBar::instance;

const TextWindow StatusBar::window =
{
    0, 0,
    LCD_WIDTH, SMALL_FONT_HEIGHT + 2,
    0, 0,
    FONT_SMALL_PROP_BOLD
};


StatusBar::StatusBar() : MessageSink(SINK_PRIORITY_SLIGHTLY_ELEVATED)
{
    MessageLoop::RegisterMessageSink(*this);
    unsigned char i;
    uint16_t* p = &battery_pips[0];
    for (i = 0; i < 4; i++)
        *p++ = lcd_Palette[COLOR_BATTERY_EMPTY_PIP];
    for (i = 0; i < 3; i++)
        *p++ = lcd_Palette[COLOR_BATTERY_PIP];
    if (Settings::GetStatusBar())
        Show();
    else
        Hide();
}


IGNORE_WARNING_UNUSED_PARAMETER
Status StatusBar::MoveTo(x_t x, y_t y)
END_IGNORE_WARNING
{
    return Status::Failure;
}



IGNORE_WARNING_UNUSED_PARAMETER
bool StatusBar::SendInput(Message& message)
END_IGNORE_WARNING
{
    return false;
}


bool StatusBar::SendMessage(Message& message)
{
    if (message.Id == MESSAGE_SETTINGS_CHANGE)
    {
        if (message.ExtendedCode == SETTINGS_STATUS_BAR_CHANGE)
            if (Settings::GetStatusBar())
                Show();
            else
                Hide();
        else
            dirty = true;
    }
    if (dirty)
        Paint();
    UpdateBatteryLevel();
    return false;
}


void StatusBar::_draw_battery_icon()
{
    if (battery_charging)
        lcd_Palette[COLOR_BATTERY_OUTLINE] = lcd_Palette[COLOR_BATTERY_CHARGING];
    else if (battery_level < 2)
        lcd_Palette[COLOR_BATTERY_OUTLINE] = lcd_Palette[COLOR_BATTERY_DISCHARGED];
    else
        lcd_Palette[COLOR_BATTERY_OUTLINE] = lcd_Palette[COLOR_BATTERY_NORMAL];
    memcpy((void*)&lcd_Palette[COLOR_BATTERY_LEVEL_1], &battery_pips[battery_level], sizeof(uint16_t) * 3);
    gfx_TransparentSprite_NoClip(battery_icon, window.X + window.Width - battery_icon_width - 1, window.Y + 1);
}


bool StatusBar::_update_battery_level()
{
    unsigned char level = battery_level;
    unsigned char charging = battery_charging;
    battery_level = boot_GetBatteryStatus();
    battery_charging = boot_IsCharging();
    battery_timer = GetRtcSeconds() + 30;
    return level != battery_level || charging != battery_charging;
}


void StatusBar::UpdateBatteryLevel()
{
    if (RtcTimer_Expired(battery_timer))
        if (_update_battery_level())
            _draw_battery_icon();
}


Status StatusBar::Paint()
{
    dirty = false;
    if (hidden)
        return Status::Success;
    //WindowSaver saver;
    window.Restore();
    fontlib_SetColors(COLOR_STATUS_BAR_FOREGROUND, COLOR_STATUS_BAR_BACKGROUND);
    fontlib_HomeUp();
    Style_SetSmallFontPropBold();
    fontlib_SetLineSpacing(1, 1);
    //fontlib_DrawString(" PROGRAMMER'S CALCULATOR");
    fontlib_DrawString(" Programmer's Calculator");
    fontlib_ClearEOL();
    Style_SetSmallFontPropBold();
    fontlib_SetLineSpacing(1, 1);
    fontlib_SetCursorPosition(150, 0);
    fontlib_DrawString(GetDisplayBitsName(Settings::GetDisplayBits()));
    fontlib_DrawString(" bits");
    fontlib_SetCursorPosition(220, 0);
    fontlib_DrawString(GetBaseShortCapsName(Settings::GetPrimaryBase()));
    fontlib_SetCursorPosition(260, 0);
    if (Settings::GetSecondaryBase() != NO_BASE)
    {
        fontlib_DrawString("(");
        fontlib_DrawString(GetBaseShortCapsName(Settings::GetSecondaryBase()));
        fontlib_DrawGlyph(')');
    }
    fontlib_SetColors(COLOR_FOREGROUND, COLOR_BACKGROUND);
    _draw_battery_icon();
    return Status::Success;
}


Status StatusBar::Show()
{
    hidden = false;
    dirty = true;
    height = SMALL_FONT_HEIGHT + 2;
    _update_battery_level();
    return Status::Success;
}


Status StatusBar::Hide()
{
    hidden = true;
    dirty = true;
    height = 0;
    //Rpn_Window.Height += SMALL_FONT_HEIGHT + 2;
    //Rpn_Window.Y = 0;
    return Status::Success;
}
