#include <fileioc.h>
#include <string.h>
#include <stdbool.h>
#include "settings.h"
#include "printbigint.h"
#include "inputbigint.h"
#include "statusbar.h"

#define SETTINGS_FILE_HEADER "Programmer's Calculator settings"
#define SETTINGS_FILE_NAME "ProgCalc"
#define VERSION_ID 1

Settings_t Settings;


typedef struct
{
    char IdString[sizeof(SETTINGS_FILE_HEADER)];
    uint8_t VersionId;
    size_t Size;
    Settings_t Settings;
} FileSettings_t;


void Settings_Initialize(void)
{
    ti_var_t file;
    FileSettings_t *fileData;
    ti_CloseAll();
    /* Initialize all settings to default values, then try to load settings from file.
     * This allows settings stored in an older version of the settings file to be loaded, while newer settings get their defaults. */
    Settings.PrimaryBase = HEXADECIMAL;
    Settings.SecondaryBase = NO_BASE;
    Settings.AlwaysShowBin = false;
    Settings.AlwaysShowOct = false;
    Settings.AlwaysShowDec = false;
    Settings.AlwaysShowHex = false;
    Settings.DisplayBits = SHOW_64;
    Settings.StatusBarEnabled = true;
    /* Look for settings file */
    file = ti_Open(SETTINGS_FILE_NAME, "r");
    do
    {
        if (!file)
            break;
        fileData = ti_GetDataPtr(file);
        if (strncmp(&fileData->IdString, SETTINGS_FILE_HEADER, sizeof(SETTINGS_FILE_HEADER)))
            break;
        memcpy(&Settings, &fileData->Settings, fileData->Size);
    } while (false);
    ti_Close(file);
    Format_ConfigureDisplaySizes();
    GetBigInt_Reposition();
    if (Settings.StatusBarEnabled)
        StatusBar_Enable();
}


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
    ti_var_t file;
    FileSettings_t *fileData;
    size_t size = sizeof(Settings_t);
    int archived = 0;
    file = ti_Open(SETTINGS_FILE_NAME, "r");
    do
    {
        if (!file)
            break;
        fileData = ti_GetDataPtr(file);
        /* TODO: Fix this warning. */
        if (strncmp(&fileData->IdString, SETTINGS_FILE_HEADER, sizeof(SETTINGS_FILE_HEADER)))
            return;
        archived = ti_IsArchived(file);
        ti_Close(file);
        /* Check if settings have changed.  If they haven't, there's no need to update anything. */
        /* We're also going to use what should be a stale pointer, but it's fine, really, just trust me. */
        if (fileData->VersionId == VERSION_ID && !memcmp(&Settings, &fileData->Settings, sizeof(Settings_t)))
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
        if (!ti_Write(&Settings, sizeof(Settings_t), 1, file))
            break;
        /* This gets run after closing GraphX specifically so that we don't have to worry about the Garbage Collect? dialog. */
        if (archived/* && ti_ArchiveHasRoom(sizeof(FileSettings_t))*/)
            ti_SetArchiveStatus(true, file);
        ti_Close(file);
        return;
    } while (false);
    ti_Close(file);
    ti_Delete(SETTINGS_FILE_NAME);
}