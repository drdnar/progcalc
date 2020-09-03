#include <fontlibc.h>
#include "statusbar.h"
#include "settings.h"
#include "rpnui.h"
#include "printbigint.h"

CharTextWindow_t StatusBar_Window =
{
    0, 0,
    LCD_WIDTH, 0,
    0, 0
};


static CharTextWindow_t oldWindow;

void StatusBar_Draw(void)
{
    char* s;
    if (!Settings.StatusBarEnabled)
        return;
    Style_SaveTextWindow(&oldWindow);
    Style_RestoreTextWindow(&StatusBar_Window);
    fontlib_SetForegroundColor(StatusBarFgColor);
    fontlib_SetBackgroundColor(StatusBarBgColor);
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
    fontlib_SetForegroundColor(FgColor);
    fontlib_SetBackgroundColor(BgColor);
    Style_RestoreTextWindow(&oldWindow);
}


void StatusBar_Enable(void)
{
    Settings.StatusBarEnabled = true;
    StatusBar_Window.Height = SMALL_FONT_HEIGHT + 2;
    Rpn_Window.Height -= SMALL_FONT_HEIGHT + 2;
    Rpn_Window.Y = SMALL_FONT_HEIGHT + 2;
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
