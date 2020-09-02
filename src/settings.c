#include "settings.h"
#include "printbigint.h"
#include "inputbigint.h"

#define SETTINGS_FILE_HEADER "Programmer's Calculator settings"

Settings_t Settings;


void Settings_Initialize(void)
{
    Settings.PrimaryBase = HEXADECIMAL;
    Settings.SecondaryBase = NO_BASE;
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
    GetBigInt_Reposition();
}


void Settings_ChangePrimaryBase(uint8_t base)
{
    if (base == Settings.PrimaryBase)
        return;
    if (base == Settings.SecondaryBase)
        Settings.SecondaryBase = Settings.PrimaryBase;
    Settings.PrimaryBase = base;
    GetBigInt_Reposition();
}


void Settings_Finalize(void)
{
    /* At some point, add some code here to save settings back to an appvar. */
}