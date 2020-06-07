#ifndef PRINT_BIG_INT_H
#define PRINT_BIG_INT_H

#include "bigint.h"
#include "style.h"
#include "settings.h"

extern Coord_t Format_BinSize;
extern Coord_t Format_OctSize;
extern Coord_t Format_DecSize;
extern Coord_t Format_HexSize;

#define MAX_FORMATTED_NUMBER_SIZE 130
#define MAX_HEX_DIGITS 32
#define MAX_DEC_DIGITS 39
#define MAX_OCT_DIGITS 43
#define MAX_BIN_DIGITS 128

#define HEX_GROUPING 4
#define DEC_GROUPING 3
#define OCT_GROUPING 3
#define BIN_GROUPING 8


/**
 * Initializes some stuff.
 */
void Format_InitDisplaySizes(void);

/**
 * Call this after changing the displayed bits setting.
 */
void Format_ConfigureDisplaySizes(void);

/**
 * Buffer for temporary storage of a number converted to ASCII for display.
 */
extern char Format_NumberBuffer[];

/**
 * Prints a number in a given base at the current FontLib cursor location using current formatting settings.
 * @param n Pointer to number to display.
 */
unsigned char Format_PrintInBase(BigInt_t* n, Base_t base);

/**
 * Prints a number in the current primary base at the current FontLib cursor location using current formatting settings.
 * @param n Pointer to number to display.
 */
unsigned char Format_PrintInPrimaryBase(BigInt_t* n);

/**
 * Prints a number in the current secondary base at the current FontLib cursor location using current formatting settings.
 * @param n Pointer to number to display.
 */
unsigned char Format_PrintInSecondaryBase(BigInt_t* n);

/**
 * Prints a number in hex at the current FontLib cursor location using current formatting settings.
 * @param n Pointer to number to display.
 */
unsigned char Format_PrintHex(BigInt_t* n);

/**
 * Prints a number in decimal at the current FontLib cursor location using current formatting settings.
 * @param n Pointer to number to display.
 */
unsigned char Format_PrintDec(BigInt_t* n);

/**
 * Prints a number in octal at the current FontLib cursor location using current formatting settings.
 * @param n Pointer to number to display.
 */
unsigned char Format_PrintOct(BigInt_t* n);

/**
 * Prints a number in binary at the current FontLib cursor location using current formatting settings.
 * @param n Pointer to number to display.
 */
unsigned char Format_PrintBin(BigInt_t* n);

#endif /* FORMATTING_H */
