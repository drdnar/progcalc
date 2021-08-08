#include <fontlibc.h>
#include <tice.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "statusbar.h"
#include "settings.h"
#include "rpnui.h"
#include "printbigint.h"
#include "forms/ez80.h"
#include "gfx/gfx.h"
#include "misc.h"
#include "forms/widget.h"
#include "forms/textmanager.h"
#include "forms/gui.h"
#include "forms/messages.h"
#include "forms/inlineoptimizations.h"

using namespace Forms;


unsigned char StatusBar::battery_level;

bool StatusBar::battery_charging;

unsigned int StatusBar::battery_timer;

uint16_t StatusBar::battery_pips[7];

StatusBar StatusBar::instance;

bool StatusBar::size_initialized = false;

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
    // Ensure GUI is resized but also ensure that happens after it is initialized.
    // This works around C++'s inability to force static class instances to be
    // initialized in a specific order, without requiring a more complicated
    // singleton implementation for GUI.
    MessageLoop::EnqueueMessage({ .Id = MESSAGE_SETTINGS_CHANGE, .ExtendedCode = SETTINGS_STATUS_BAR_CHANGE });
}


Status StatusBar::MoveTo([[maybe_unused]] x_t x, [[maybe_unused]] y_t y)
{
    return Status::Failure;
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
    else if (message.Id == MESSAGE_REPAINT_ALL)
        SetDirty();
    if (dirty)
        Paint();
    UpdateBatteryLevel();
    return false;
}


void StatusBar::_update_battery_icon()
{
    if (battery_charging)
        lcd_Palette[COLOR_BATTERY_OUTLINE] = lcd_Palette[COLOR_BATTERY_CHARGING];
    else if (battery_level < 2)
        lcd_Palette[COLOR_BATTERY_OUTLINE] = lcd_Palette[COLOR_BATTERY_DISCHARGED];
    else
        lcd_Palette[COLOR_BATTERY_OUTLINE] = lcd_Palette[COLOR_BATTERY_NORMAL];
    memcpy_inline_nonzero((void*)&lcd_Palette[COLOR_BATTERY_LEVEL_1], &battery_pips[battery_level], sizeof(uint16_t) * 3);
}


bool StatusBar::_update_battery_level()
{
    unsigned char level = battery_level;
    unsigned char charging = battery_charging;
    battery_level = boot_GetBatteryStatus();
    battery_charging = boot_IsCharging();
    battery_timer = GetRtcSeconds() + 30;
    if (battery_level == 0 && !battery_charging)
        MessageLoop::EnqueueMessage({ .Id = MESSAGE_EXIT_EVENT_LOOP, .ExtendedCode = MESSAGE_NONE });
    return level != battery_level || charging != battery_charging;
}


void StatusBar::UpdateBatteryLevel()
{
    if (RtcTimer_Expired(battery_timer))
        if (_update_battery_level())
            _update_battery_icon();
}


Status StatusBar::Paint()
{
    dirty = false;
    if (hidden)
        return Status::Success;
    window.Restore();
    fontlib_SetColors(COLOR_STATUS_BAR_FOREGROUND, COLOR_STATUS_BAR_BACKGROUND);
    fontlib_HomeUp();
    FontManager::SetFont(FONT_SMALL_PROP_BOLD);
    fontlib_SetLineSpacing(1, 1);
    fontlib_DrawString(" Programmer's Calculator v" VERSION_NUMBER);
    fontlib_ClearEOL();
    fontlib_SetCursorPosition(176, 0);
    fontlib_DrawString(DisplayBitsNames.Get(Settings::GetDisplayBits()));
    fontlib_DrawString(" bits");
    fontlib_SetCursorPosition(230, 0);
    fontlib_DrawString(BaseShortCapNames.Get(Settings::GetPrimaryBase()));
    fontlib_SetCursorPosition(260, 0);
    if (Settings::GetSecondaryBase() != NO_BASE)
    {
        fontlib_DrawString("(");
        fontlib_DrawString(BaseShortCapNames.Get(Settings::GetSecondaryBase()));
        fontlib_DrawGlyph(')');
    }
    fontlib_SetColors(COLOR_FOREGROUND, COLOR_BACKGROUND);
    _update_battery_icon();
    gfx_TransparentSprite_NoClip(battery_icon, window.X + window.Width - battery_icon_width - 1, window.Y + 1);
    return Status::Success;
}


Status StatusBar::Show()
{
    if (size_initialized && !hidden)
        return Status::Success;
    hidden = false;
    dirty = true;
    height = SMALL_FONT_HEIGHT + 2;
    _update_battery_level();
    auto& gui = GUI::GetInstance();
    gui.SetHeight(gui.GetHeight() - height);
    gui.MoveTo(0, height);
    size_initialized = true;
    return Status::Success;
}


Status StatusBar::Hide()
{
    if (size_initialized && hidden)
        return Status::Success;
    if (size_initialized)
    {
        auto& gui = GUI::GetInstance();
        gui.SetHeight(gui.GetHeight() + height);
        gui.MoveTo(0, 0);
    }
    hidden = true;
    dirty = true;
    height = 0;
    size_initialized = true;
    return Status::Success;
}
