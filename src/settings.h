#ifndef SETTINGS_H
#define SETTINGS_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef enum
{
    BINARY,
    OCTAL,
    DECIMAL,
    HEXADECIMAL,
    NO_BASE = 255
} Bases_t;

typedef uint8_t Base_t;

typedef enum
{
    SHOW_32,
    SHOW_64,
    SHOW_128
} DisplaySizes_t;

typedef struct
{
    uint8_t DisplayBits;
    Base_t PrimaryBase;
    Base_t SecondaryBase;
    bool AlwaysShowHex;
    bool AlwaysShowDec;
    bool AlwaysShowOct;
    bool AlwaysShowBin;
} Settings_t;

extern Settings_t Settings;

/**
 * Performs initialization of settings at program startup.
 */
void Settings_Initialize(void);

/**
 * Performs clean-up of settings, namely saving them back to an appvar.
 */
void Settings_Finalize(void);

/**
 * Changes the number of bits/bytes to display.
 * Triggers any other events that should be triggered when this setting is changed.
 * @param bytes Member of DisplaySizes_t
 */
void Settings_ChangeDisplayBits(uint8_t bytes);

#endif /* SETTINGS_H */
