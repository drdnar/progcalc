#ifndef PRINT_BIG_INT_H
#define PRINT_BIG_INT_H

#include "bigint.h"
#include "settings.h"
#include "forms/textmanager.h"

#ifdef __cplusplus
extern "C" {
#endif

extern Forms::Coord Format_BinSize;
extern Forms::Coord Format_OctSize;
extern Forms::Coord Format_DecSize;
extern Forms::Coord Format_HexSize;

#define MAX_FORMATTED_NUMBER_SIZE 130
#define MAX_HEX_DIGITS 32
#define MAX_DEC_DIGITS 39
#define MAX_OCT_DIGITS 43
#define MAX_BIN_DIGITS 128

#define HEX_GROUPING 4
#define DEC_GROUPING 3
#define OCT_GROUPING 3
#define BIN_GROUPING 8

#define CHAR_WIDTH 8
#define CHAR_HEIGHT 14

#define BIG_FONT_HEIGHT 14
#define SMALL_FONT_HEIGHT 10

#define GROUP_DELIMITER ' '

/**
 * Initializes some stuff.
 */
void Format_InitDisplaySizes();

/**
 * Call this after changing the displayed bits setting.
 */
void Format_ConfigureDisplaySizes();

/**
 * Buffer for temporary storage of a number converted to ASCII for display.
 */
extern char Format_NumberBuffer[];

/**
 * Returns the height, in pixels, of a formatted number with current formatting settings.
 * @param base Radix of number
 * @return Height in pixels
 */
unsigned char Format_GetNumberHeight(Base_t base);

/**
 * Gets the height of an entry, taking into account both primary and secondary
 * base settings.
 * @return Height in pixels.
 */
unsigned char Format_GetEntryHeight();

/**
 * Prints a number using current settings for bits and primary/secondary base.
 * @param n Pointer to number to print.
 */
void Format_PrintEntry(BigInt_t* n);

/**
 * Gets the height of an entry, taking into account both primary and secondary
 * base settings, as well as AlwaysShow settings.
 * @return Height in pixels.
 */
unsigned char Format_GetEntryWithAlwaysShowsHeight();

/**
 * Like Format_PrintEntry() but also handles AlwaysShow settings.
 * @param n Pointer to number to print.
 */
void Format_PrintEntryWithAlwaysShows(BigInt_t* n);

/**
 * Prints a number, right-aligned, in a given base at the current FontLib cursor location using current formatting settings,
 * and also erases the space to the left.
 * @param n Pointer to number to display.
 * @return X coordinate printing started at.
 */
void Format_PrintInBase(BigInt_t* n, Base_t base);

/**
 * Prints a number, right-aligned, in hex at the current FontLib cursor location using current formatting settings.
 * @param n Pointer to number to display.
 * @return X coordinate printing started at.
 */
unsigned int Format_PrintHex(BigInt_t* n);

/**
 * Prints a number, right-aligned, in decimal at the current FontLib cursor location using current formatting settings.
 * @param n Pointer to number to display.
 * @return X coordinate printing started at.
 */
unsigned int Format_PrintDec(BigInt_t* n);

/**
 * Prints a number, right-aligned, in octal at the current FontLib cursor location using current formatting settings.
 * @param n Pointer to number to display.
 * @return X coordinate printing started at.
 */
unsigned int Format_PrintOct(BigInt_t* n);

/**
 * Prints a number, right-aligned, in binary at the current FontLib cursor location using current formatting settings.
 * @param n Pointer to number to display.
 * @return X coordinate printing started at.
 */
unsigned int Format_PrintBin(BigInt_t* n);

#ifdef __cplusplus
}
#endif

#endif /* FORMATTING_H */
