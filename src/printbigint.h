#ifndef PRINT_BIG_INT_H
#define PRINT_BIG_INT_H

#include "bigint.h"
#include "style.h"

extern CharCoord_t Format_BinSize;
extern CharCoord_t Format_OctSize;
extern CharCoord_t Format_DecSize;
extern CharCoord_t Format_HexSize;

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
 * Call this after changing the displayed bits setting.
 */
void Format_ConfigureDisplaySizes(void);

/**
 * Buffer for temporary storage of a number converted to ASCII for display.
 */
extern char Format_NumberBuffer[];

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
