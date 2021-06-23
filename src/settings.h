#ifndef SETTINGS_H
#define SETTINGS_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>


enum SettingsMessageIds
{
    SETTINGS_DISPLAY_BITS_CHANGE,
    SETTINGS_PRIMARY_BASE_CHANGE,
    SETTINGS_SECONDARY_BASE_CHANGE,
    SETTINGS_ALWAYS_SHOW_HEX_CHANGE,
    SETTINGS_ALWAYS_SHOW_DEC_CHANGE,
    SETTINGS_ALWAYS_SHOW_OCT_CHANGE,
    SETTINGS_ALWAYS_SHOW_BIN_CHANGE,
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
        static inline bool GetDisplayBits(void) { return settings.DisplayBits; }
        /**
         * DisplayBits setter.
         */
        static void SetDisplayBits(uint8_t bits);
        /**
         * Getter for PrimaryBase.
         * @return Member of { BINARY, OCTAL, DECIMAL, HEXADECIMAL }
         */
        static Base_t GetPrimaryBase(void) { return settings.PrimaryBase; }
        /**
         * Setter for PrimaryBase.
         */
        static void SetPrimaryBase(Base_t base);
        /**
         * Getter for SecondaryBase.
         * @return Member of { BINARY, OCTAL, DECIMAL, HEXADECIMAL }
         */
        static Base_t GetSecondaryBase(void) { return settings.SecondaryBase; }
        /**
         * Setter for SecondaryBase.
         */
        static void SetSecondaryBase(Base_t base);
        /**
         * AlwaysShow base getter
         */
        static bool AlwaysShowHex(void) { return settings.AlwaysShowHex; }
        /**
         * AlwaysShow base setter
         */
        static void SetAlwaysShowHex(bool value);
        /**
         * AlwaysShow base getter
         */
        static bool AlwaysShowDec(void) { return settings.AlwaysShowDec; }
        /**
         * AlwaysShow base setter
         */
        static void SetAlwaysShowDec(bool value);
        /**
         * AlwaysShow base getter
         */
        static bool AlwaysShowOct(void) { return settings.AlwaysShowOct; }
        /**
         * AlwaysShow base setter
         */
        static void SetAlwaysShowOct(bool value);
        /**
         * AlwaysShow base getter
         */
        static bool AlwaysShowBin(void) { return settings.AlwaysShowBin; }
        /**
         * AlwaysShow base setter
         */
        static void SetAlwaysShowBin(bool value);
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
};



#ifdef __cplusplus
extern "C" {
#endif

/**
 * Returns a string that gives the short name of a base.
 */
char* GetBaseShortName(Base_t base);

/**
 * Returns a string that gives the short name of a base, in ALL CAPS.
 */
char* GetBaseShortCapsName(Base_t base);

/**
 * Returns a string that gives the long name of a base.
 */
char* GetBaseLongName(Base_t base);

/**
 * Converts a DisplayBits const into a string.
 */
char* GetDisplayBitsName(uint8_t bytes);

#ifdef __cplusplus
}
#endif

#endif /* SETTINGS_H */
