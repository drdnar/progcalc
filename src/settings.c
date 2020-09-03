#include "settings.h"
#include "printbigint.h"
#include "inputbigint.h"
#include "statusbar.h"

#define SETTINGS_FILE_HEADER "Programmer's Calculator settings"

Settings_t Settings;


void Settings_Initialize(void)
{
    Settings.PrimaryBase = NO_BASE;
    Settings_ChangePrimaryBase(HEXADECIMAL);
    Settings.SecondaryBase = NO_BASE;
    Settings.AlwaysShowBin = false;
    Settings.AlwaysShowOct = false;
    Settings.AlwaysShowDec = false;
    Settings.AlwaysShowHex = false;
    Settings_ChangeDisplayBits(SHOW_64);
    StatusBar_Enable();
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


char* displayBitsNames[] = 
{
    "32", "64", "128"
};

char* GetDisplayBitsName(uint8_t bytes)
{
    return displayBitsNames[bytes];
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


static char* shortBaseNames[] =
{
    "bin", "oct", "dec", "hex", ""
};

static char* shortCapsBaseNames[] =
{
    "BIN", "OCT", "DEC", "HEX", ""
};

static char* longBaseNames[] =
{
    "Binary", "Octal", "Decimal", "Hexadecimal", ""
};

char* GetBaseShortName(Base_t base)
{
    return shortBaseNames[base];
}

char* GetBaseShortCapsName(Base_t base)
{
    return shortCapsBaseNames[base];
}

char* GetBaseLongName(Base_t base)
{
    return longBaseNames[base];
}


void Settings_Finalize(void)
{
    /* At some point, add some code here to save settings back to an appvar. */
}