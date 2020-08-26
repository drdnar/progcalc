#include "settings.h"
#include "printbigint.h"

#define SETTINGS_FILE_HEADER "Programmer's Calculator settings"

Settings_t Settings;


void Settings_Initialize(void)
{
    Settings.PrimaryBase = HEXADECIMAL;
    Settings.SecondaryBase = DECIMAL;
    Settings.AlwaysShowBin = false;
    Settings.AlwaysShowOct = false;
    Settings.AlwaysShowDec = false;
    Settings.AlwaysShowHex = false;
    Settings_ChangeDisplayBits(SHOW_64);
}


/*    uint8_t DisplayBits;
    Base_t PrimaryBase;
    Base_t SecondaryBase;
    bool AlwaysShowHex;
    bool AlwaysShowDec;
    bool AlwaysShowOct;
    bool AlwaysShowBin;*/

void Settings_ChangeDisplayBits(uint8_t bytes)
{
    if (bytes == Settings.DisplayBits)
        return;
    Settings.DisplayBits = bytes;
    Format_ConfigureDisplaySizes();
}


void Settings_Finalize(void)
{
    /* At some point, add some code here to save settings back to an appvar. */
}