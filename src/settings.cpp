#include <fileioc.h>
#include <string.h>
#include <stdbool.h>

#include "settings.h"
#include "printbigint.h"
#include "inputbigint.h"
#include "statusbar.h"
#include "forms/messages.h"
#include "forms/inlineoptimizations.h"

#define SETTINGS_FILE_HEADER "Programmer's Calculator settings"
#define SETTINGS_FILE_NAME "ProgCalc"
#define VERSION_ID 1

using namespace Forms;

Settings Settings::instance;
Settings_t Settings::settings
{
    .DisplayBits = SHOW_64,
    .PrimaryBase = HEXADECIMAL,
    .SecondaryBase = NO_BASE,
    .AlwaysShowHex = false,
    .AlwaysShowDec = false,
    .AlwaysShowOct = false,
    .AlwaysShowBin = false,
    .StatusBarEnabled = true
};


struct FileSettings_t
{
    char IdString[sizeof(SETTINGS_FILE_HEADER)];
    uint8_t VersionId;
    size_t Size;
    Settings_t Settings;
};


Settings::Settings()
{
    ti_var_t file;
    FileSettings_t *fileData;
    ti_CloseAll();
    /* Look for settings file */
    file = ti_Open(SETTINGS_FILE_NAME, "r");
    do
    {
        if (!file)
            break;
        fileData = (FileSettings_t*)ti_GetDataPtr(file);
        if (strncmp((char*)&fileData->IdString, SETTINGS_FILE_HEADER, sizeof(SETTINGS_FILE_HEADER)))
            break;
        memcpy_inline_nonzero(&settings, &fileData->Settings, fileData->Size);
    } while (false);
    ti_Close(file);
    /** TODO: if (Settings.StatusBarEnabled)
        StatusBar_Enable();*/
}


Settings::~Settings()
{
    ti_var_t file;
    FileSettings_t *fileData;
    size_t size = sizeof(Settings_t);
    bool archived = false;
    file = ti_Open(SETTINGS_FILE_NAME, "r");
    do
    {
        if (!file)
            break;
        fileData = (FileSettings_t*)ti_GetDataPtr(file);
        /* TODO: Fix this warning. */
        //int strncmp(const char *, const char *, size_t);
        if (strncmp((char*)&fileData->IdString, SETTINGS_FILE_HEADER, sizeof(SETTINGS_FILE_HEADER)))
            return;
        archived = (bool)ti_IsArchived(file);
        ti_Close(file);
        /* Check if settings have changed.  If they haven't, there's no need to update anything. */
        /* We're also going to use what should be a stale pointer, but it's fine, really, just trust me. */
        if (fileData->VersionId == VERSION_ID && !memcmp(&settings, &fileData->Settings, sizeof(Settings_t)))
            return;
        if (!ti_Delete(SETTINGS_FILE_NAME))
            return;
    } while (false);
    ti_Close(file);
    file = ti_Open(SETTINGS_FILE_NAME, "w");
    do
    {
        if (!file)
        {
            ti_Close(file);
            return;
        }
        if (!ti_Write(SETTINGS_FILE_HEADER, sizeof(SETTINGS_FILE_HEADER), 1, file))
            break;
        if (!ti_PutC(VERSION_ID, file))
            break;
        if (!ti_Write(&size, sizeof(size), 1, file))
            break;
        if (!ti_Write(&settings, sizeof(Settings_t), 1, file))
            break;
        if (archived/* && ti_ArchiveHasRoom(sizeof(FileSettings_t))*/)
        {
            gfx_End();
            ti_SetArchiveStatus(true, file);
        }
        ti_Close(file);
        return;
    } while (false);
    ti_Close(file);
    ti_Delete(SETTINGS_FILE_NAME);
}


void Settings::SetDisplayBits(uint8_t bits)
{
    if (bits == settings.DisplayBits)
        return;
    settings.DisplayBits = bits;
    MessageLoop::EnqueueMessage({ .Id = MESSAGE_SETTINGS_CHANGE, .ExtendedCode = SETTINGS_DISPLAY_BITS_CHANGE });
    /** TODO: These need to catch the settings change messages instead. */
    Format_ConfigureDisplaySizes();
}


void Settings::SetPrimaryBase(Base_t base)
{
    if (base == settings.PrimaryBase)
        return;
    /** TODO: These need to catch the settings change messages instead. */
    if (base == settings.SecondaryBase)
    {
        settings.SecondaryBase = settings.PrimaryBase;
        MessageLoop::EnqueueMessage({ .Id = MESSAGE_SETTINGS_CHANGE, .ExtendedCode = SETTINGS_SECONDARY_BASE_CHANGE });
    }
    settings.PrimaryBase = base;
    MessageLoop::EnqueueMessage({ .Id = MESSAGE_SETTINGS_CHANGE, .ExtendedCode = SETTINGS_PRIMARY_BASE_CHANGE });
}


void Settings::SetSecondaryBase(Base_t base)
{
    if (base == settings.SecondaryBase)
        return;
    settings.SecondaryBase = base;
    MessageLoop::EnqueueMessage({ .Id = MESSAGE_SETTINGS_CHANGE, .ExtendedCode = SETTINGS_SECONDARY_BASE_CHANGE });
}


void Settings::SetAlwaysShowHex(bool value)
{
    if (value == settings.AlwaysShowHex)
        return;
    settings.AlwaysShowHex = value;
    MessageLoop::EnqueueMessage({ .Id = MESSAGE_SETTINGS_CHANGE, .ExtendedCode = SETTINGS_ALWAYS_SHOW_HEX_CHANGE });
}


void Settings::SetAlwaysShowDec(bool value)
{
    if (value == settings.AlwaysShowDec)
        return;
    settings.AlwaysShowDec = value;
    MessageLoop::EnqueueMessage({ .Id = MESSAGE_SETTINGS_CHANGE, .ExtendedCode = SETTINGS_ALWAYS_SHOW_DEC_CHANGE });
}


void Settings::SetAlwaysShowOct(bool value)
{
    if (value == settings.AlwaysShowOct)
        return;
    settings.AlwaysShowOct = value;
    MessageLoop::EnqueueMessage({ .Id = MESSAGE_SETTINGS_CHANGE, .ExtendedCode = SETTINGS_ALWAYS_SHOW_OCT_CHANGE });
}


void Settings::SetAlwaysShowBin(bool value)
{
    if (value == settings.AlwaysShowBin)
        return;
    settings.AlwaysShowBin = value;
    MessageLoop::EnqueueMessage({ .Id = MESSAGE_SETTINGS_CHANGE, .ExtendedCode = SETTINGS_ALWAYS_SHOW_BIN_CHANGE });
}


void Settings::SetStatusBar(bool value)
{
    if (value == settings.StatusBarEnabled)
        return;
    settings.StatusBarEnabled = value;
    MessageLoop::EnqueueMessage({ .Id = MESSAGE_SETTINGS_CHANGE, .ExtendedCode = SETTINGS_STATUS_BAR_CHANGE });
}


const char* Settings::Apply(Settings_t& newSettings)
{
    if (newSettings.PrimaryBase == NO_BASE)
        return "Invalid primary base.";
    if (newSettings.PrimaryBase == newSettings.SecondaryBase)
        return "Primary and secondary are same.";
    if (newSettings.StatusBarEnabled != settings.StatusBarEnabled)
        SetStatusBar(newSettings.StatusBarEnabled);
    if (newSettings.PrimaryBase != settings.PrimaryBase)
        SetPrimaryBase(newSettings.PrimaryBase);
    if (newSettings.SecondaryBase != settings.SecondaryBase)
        SetSecondaryBase(newSettings.SecondaryBase);
    if (newSettings.DisplayBits != settings.DisplayBits)
        SetDisplayBits(newSettings.DisplayBits);
    if (newSettings.AlwaysShowBin != settings.AlwaysShowBin)
        SetAlwaysShowBin(newSettings.AlwaysShowBin);
    if (newSettings.AlwaysShowOct != settings.AlwaysShowOct)
        SetAlwaysShowOct(newSettings.AlwaysShowOct);
    if (newSettings.AlwaysShowDec != settings.AlwaysShowDec)
        SetAlwaysShowDec(newSettings.AlwaysShowDec);
    if (newSettings.AlwaysShowHex != settings.AlwaysShowHex)
        SetAlwaysShowHex(newSettings.AlwaysShowHex);
    return nullptr;
}


/* Please forgive this syntax. */
static const MapTableEntry baseShortNames[] = 
{
    { BINARY, {.CPtr = "bin"} },
    { OCTAL, {.CPtr = "oct"} },
    { DECIMAL, {.CPtr = "dec"} },
    { HEXADECIMAL, {.CPtr = "hex"} },
};
static ConstMapTable baseShortNames_ = MAP_TABLE(.CPtr = "?", const char*, baseShortNames);
const EnumToString BaseShortNames { baseShortNames_, sizeof(Base_t) };


static const MapTableEntry baseShortCapNames[] = 
{
    { BINARY, {.CPtr = "BIN"} },
    { OCTAL, {.CPtr = "OCT"} },
    { DECIMAL, {.CPtr = "DEC"} },
    { HEXADECIMAL, {.CPtr = "HEX"} },
};
static ConstMapTable baseShortCapNames_ = MAP_TABLE(.CPtr = "?", const char*, baseShortCapNames);
const EnumToString BaseShortCapNames { baseShortCapNames_, sizeof(Base_t) };


static const MapTableEntry baseLongNames[] = 
{
    { BINARY, {.CPtr = "Binary"} },
    { OCTAL, {.CPtr = "Octal"} },
    { DECIMAL, {.CPtr = "Decimal"} },
    { HEXADECIMAL, {.CPtr = "Hexadecimal"} },
    { HEXADECIMAL + 1, {.CPtr = "Unspecified"} },
};
static ConstMapTable baseLongNames_ = MAP_TABLE(.CPtr = "?", const char*, baseLongNames);
const EnumToString BaseLongNames { baseLongNames_, sizeof(Base_t) };


static const MapTableEntry displayBitsNames_a[] = 
{
    { BINARY, {.CPtr = "32"} },
    { OCTAL, {.CPtr = "64"} },
    { DECIMAL, {.CPtr = "128"} },
};
static ConstMapTable displayBitsNames_ = MAP_TABLE(.CPtr = "?", const char*, displayBitsNames_a);
const EnumToString DisplayBitsNames { displayBitsNames_, sizeof(uint8_t) };
