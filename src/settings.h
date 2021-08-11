#ifndef SETTINGS_H
#define SETTINGS_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "forms/dialogbox.h"
#include "forms/enumtostring.h"


enum SettingsMessageIds
{
    SETTINGS_DISPLAY_BITS_CHANGE,
    SETTINGS_PRIMARY_BASE_CHANGE,
    SETTINGS_SECONDARY_BASE_CHANGE,
    SETTINGS_ALWAYS_SHOW_HEX_CHANGE,
    SETTINGS_ALWAYS_SHOW_DEC_CHANGE,
    SETTINGS_ALWAYS_SHOW_OCT_CHANGE,
    SETTINGS_ALWAYS_SHOW_BIN_CHANGE,
    SETTINGS_STATUS_BAR_CHANGE,
    SETTINGS_SIGNED_MODE_CHANGE,
};

enum Bases_t
{
    BINARY,
    OCTAL,
    DECIMAL,
    HEXADECIMAL,
    NO_BASE
};

typedef uint8_t Base_t;

enum DisplaySizes_t
{
    SHOW_32,
    SHOW_64,
    SHOW_128
};

struct Settings_t final
{
    uint8_t DisplayBits;
    Base_t PrimaryBase;
    Base_t SecondaryBase;
    bool AlwaysShowHex;
    bool AlwaysShowDec;
    bool AlwaysShowOct;
    bool AlwaysShowBin;
    bool StatusBarEnabled;
    bool FirstRun;
    bool SaveStack;
    bool SignedMode;
};


/**
 * Global settings manager.  Automatically saves and loads settings.
 *
 * By the time the GUI subsystem initializes the calculator UI, all settings
 * have already been loaded.
 */
class Settings final
{
    public:
        /**
         * DisplayBits getter.
         * @return Member of { SHOW_32, SHOW_64, SHOW_128 }
         */
        static inline Base_t GetDisplayBits() { return settings.DisplayBits; }
        /**
         * DisplayBits setter.
         */
        static void SetDisplayBits(uint8_t bits);
        /**
         * Getter for PrimaryBase.
         * @return Member of { BINARY, OCTAL, DECIMAL, HEXADECIMAL }
         */
        static Base_t GetPrimaryBase() { return settings.PrimaryBase; }
        /**
         * Setter for PrimaryBase.
         */
        static void SetPrimaryBase(Base_t base);
        /**
         * Getter for SecondaryBase.
         * @return Member of { BINARY, OCTAL, DECIMAL, HEXADECIMAL }
         */
        static Base_t GetSecondaryBase() { return settings.SecondaryBase; }
        /**
         * Setter for SecondaryBase.
         */
        static void SetSecondaryBase(Base_t base);
        /**
         * AlwaysShow base getter
         */
        static bool AlwaysShowHex() { return settings.AlwaysShowHex; }
        /**
         * AlwaysShow base setter
         */
        static void SetAlwaysShowHex(bool value);
        /**
         * AlwaysShow base getter
         */
        static bool AlwaysShowDec() { return settings.AlwaysShowDec; }
        /**
         * AlwaysShow base setter
         */
        static void SetAlwaysShowDec(bool value);
        /**
         * AlwaysShow base getter
         */
        static bool AlwaysShowOct() { return settings.AlwaysShowOct; }
        /**
         * AlwaysShow base setter
         */
        static void SetAlwaysShowOct(bool value);
        /**
         * AlwaysShow base getter
         */
        static bool AlwaysShowBin() { return settings.AlwaysShowBin; }
        /**
         * AlwaysShow base setter
         */
        static void SetAlwaysShowBin(bool value);
        /**
         * Checks for StatusBar display
         */
        static bool GetStatusBar() { return settings.StatusBarEnabled; }
        /**
         * Checks if the stack should be saved.
         */
        static bool SaveStack() { return settings.SaveStack; }
        /**
         * Controls whether the stack will be saved on quit and restored on
         * resume.
         */
        static void SetSaveStack(bool value) { settings.SaveStack = value; }
        /**
         * Enables/disables StatusBar.
         */
        static void SetStatusBar(bool value);
        /**
         * Check if this is the first time running the program.
         */
        static bool IsFirstRun() { return settings.FirstRun; }
        /**
         * Checks if numbers should be displayed as signed.
         */
        static bool GetSignedMode() { return settings.SignedMode; }
        /**
         * Sets whether numbers are displayed as signed.
         */
        static void SetSignedMode(bool value);
        /**
         * Applies changed settings.
         * @return Returns nullptr if the settings are valid.
         * If settings are invalid, no changes are made and a string describing
         * the error is returned.
         */
        static const char* Apply(Settings_t& newSettings);
        /**
         * Copies settings data to another location.
         */
        static void CopyTo(Settings_t& target) { target = settings; }
    private:
        Settings(void);
        ~Settings();
        /**
         * Hook to load settings on start and save on quit.
         */
        static Settings instance;
        /**
         * A struct that can be memcpy'd.
         */
        static Settings_t settings;
        friend void SettingsLoadSettings();
};

/**
 * Mappings of base_t to a short base name, e.g. "hex".
 */
extern const Forms::EnumToString BaseShortNames;
/**
 * Mappings of base_t to a short base name in ALL CAPS, e.g. "HEX".
 */
extern const Forms::EnumToString BaseShortCapNames;
/**
 * Mappings of base_t to a long base name, e.g. "Hexadecimal".
 */
extern const Forms::EnumToString BaseLongNames;
/**
 * Mappings of display bits values to a string, e.g. "64" for SHOW_64.
 */
extern const Forms::EnumToString DisplayBitsNames;

#endif /* SETTINGS_H */
