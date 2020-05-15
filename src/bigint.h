#ifndef BIG_INT_H
#define BIG_INT_H
#define BIG_INT_SIZE 16

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	uint8_t d[BIG_INT_SIZE];
} BigInt_t;

extern const BigInt_t BigIntZero;
extern const BigInt_t BigIntTen;

/**
 * Copies a BigInt.
 * @param source Pointer to BigInt to copy
 * @param destination Pointer to BigInt to overwrite
 */
/*void BigIntCopyFromTo(const BigInt_t* source, BigInt_t* destination);*/
#define BigIntCopyFromTo(src, dst) memcpy(dst, src, sizeof(BigInt_t))

/**
 * Zeros-out a BigInt.
 * n = 0
 * @param n Pointer to BigInt to zero-out
 */
void BigIntSetToZero(BigInt_t* n);

/**
 * Sets a BigInt to one.
 * n = 1
 * @param n Pointer to BigInt
 */
void BigIntSetToOne(BigInt_t* n);

/**
 * Sets a BigInt to negative one.
 * n = -1
 * @param n Pointer to BigInt
 */
void BigIntSetToNegativeOne(BigInt_t* n);

/**
 * Increments a BigInt in-place.
 * n++
 * @param n Pointer to number to increment.
 * @return Returns any non-zero number upon overflow
 */
uint8_t BigIntIncrement(BigInt_t* n);

/**
 * Decrements a BigInt in-place.
 * n--
 * @param n Pointer to number to decrement.
 * @return Returns any non-zero number upon underflow
 */
uint8_t BigIntDecrement(BigInt_t* n);

/**
 * Adds two BigInts together and overwrites the first with the sum
 * n1 += n2
 * @param n1 Pointer to first number
 * @param n2 Pointer to second number
 * @return Returns non-zero on overflow
 */
uint8_t BigIntAdd(BigInt_t* n1, const BigInt_t* n2);

/**
 * Subtracts two BigInts and overwrites the first with the difference
 * n1 -= n2
 * @param n1 Pointer to minuend and location to write difference
 * @param n2 Pointer to subtrahend
 * @return Returns non-zero on underflow
 */
int8_t BigIntSubtract(BigInt_t* n1, const BigInt_t* n2);

/**
 * Negates a BigInt.
 * n = -n
 * @param n Pointer to number to negate
 */
void BigIntNegate(BigInt_t* n);

/**
 * Checks if a BigInt is NOT zero.
 * @param n Pointer to number to check
 * @return 0 if n is zero, ANY non-zero value if n is non-zero
 * @note If n is non-zero, then any number between 1 and 255 may be returned.
 */
uint8_t BigIntIsNonZero(const BigInt_t* n);

/**
 * Checks if a BigInt is zero.
 * @param n Pointer to number to check
 * @return 0 if n is non-zero, 1 if n is zero
 */
bool BigIntIsZero(const BigInt_t* n);

/**
 * Returns the sign of a number.
 * @param n Pointer to number to get sign of
 * @return 1 is n is positive, 0 if n is zero, -1 if n is negative
 */
int8_t BigIntGetSign(const BigInt_t* n);

/**
 * Compares n1 to n2, UNSIGNED.
 * @param n1 Pointer to a number
 * @param n2 Pointer to number to compare n1 with
 * @return Returns a positive number if n1 > n2, 0 if n1 == n2, and a negative number if n1 < n2.
 * @note Return value can be any positive or negative number, not just 1, 0, or -1.
 */
int8_t BigIntCompare(const BigInt_t* n1, const BigInt_t* n2);

/**
 * Multiplies two BigInts.
 * out = n1 * n2
 * @note Truncates result to a single BigInt
 * @param n1 Pointer to multiplicand
 * @param n2 Pointer to multiplier
 * @param out Pointer to BigInt to write result to
 * @return Returns a non-zero number on overflow.
 */
uint8_t BigIntMultiply(const BigInt_t* n1, const BigInt_t* n2, BigInt_t* out);

/**
 * Divides two BigInts.
 * q = int(n / d)
 * r = n % d
 * @param n Pointer to numerator/dividend
 * @param d Pointer to denominator/divisor
 * @param q Pointer to BigInt to write quotient to
 * @param r Pointer to BigInt to write remainder to
 */
void BigIntDivide(const BigInt_t* n, const BigInt_t* d, BigInt_t* q, BigInt_t* r);

/**
 * Writes a binary ASCII string representation of a byte into a string.
 * @param n Byte to turn into ASCII
 * @param buffer Location to write eight chars to.
 * @return  Pointer to '\0' at end of written string, i.e. buffer + 8
 */
char* BigIntBinify(const uint8_t n, char* buffer);

/**
 * Writes an octal ASCII string representation of a byte into a string.
 * @param n Byte to turn into ASCII
 * @param buffer Location to write three chars to.
 * @return  Pointer to '\0' at end of written string, i.e. buffer + 3
 */
char* BigIntOctify(const uint8_t n, char* buffer);

/**
 * Classic optimized routine.
 * @param n Byte to turn into ASCII hex
 * @param buffer Location to write two chars to
 * @return Pointer to '\0' at end of written string, i.e. buffer + 2
 */
char* BigIntHexify(const uint8_t n, char* buffer);

/**
 * Writes to buffer a string representing a BigInt in binary.
 * @param n BigInt to convert to binary ASCII
 * @param buffer 
 */
char* BigIntToStringBin(const BigInt_t* n, char* buffer);

/**
 * Writes to buffer a string representing a BigInt in octal.
 * @param n BigInt to turn into octal
 * @param buffer 
 */
char* BigIntToStringHex(const BigInt_t* n, char* buffer);

/**
 * Writes to buffer a string representing a BigInt.
 * @param n BigInt to hexify
 * @param buffer 
 */
char* BigIntToStringHex(const BigInt_t* n, char* buffer);

/**
 * Converts an unsigned BigInt into a decimal string.
 * This routine is slow and uses a fair bit of stack space.
 * @param n Pointer to BigInt
 * @param buffer Pointer to buffer to write string
 * @return Pointer to '\0' at end of written string
 */
char* BigIntToString(const BigInt_t* n, char* buffer);

/**
 * Shifts a BigInt left one bit.
 * n <<= 1
 * @param n Pointer to number
 * @return Returns left-most bit (0 or 1)
 */
uint8_t BigIntShiftLeft(BigInt_t* n);

/**
 * Shifts a BigInt left one bit and puts another bit into it.
 * n = (n << 1) | v
 * @param n Pointer to number
 * @param v Bit to shift in, either 0 or 1
 * @return Returns left-most bit (0 or 1)
 */
uint8_t BigIntShiftBitInOnLeft(BigInt_t* n, uint8_t v);

/**
 * Shifts a BigInt right one bit.  The top bit is reset.
 * n >>= 1
 * @param n Pointer to number
 * @return Returns right-most bit (0 or 1)
 */
uint8_t BigIntShiftRight(BigInt_t* n);

/**
 * Shifts a BigInt right one bit.  Sign is preserved.
 * n >>= 1, sign of n is preserved
 * @param n Pointer to number
 * @return Returns right-most bit (0 or 1)
 */
uint8_t BigIntSignedShiftRight(BigInt_t* n);

/**
 * Shifts a BigInt right one bit and puts another bit into it.
 * n = (n >> 1) | (v << (BIG_INT_SIZE - 1))
 * @param n Pointer to number
 * @param v Bit to shift in, either 0 or 1
 * @return Returns right-most bit (0 or 1)
 */
uint8_t BigIntShiftBitInOnRight(BigInt_t* n, uint8_t v);

/**
 * Gets bit b from n.
 * @param n Pointer to number
 * @param b Bit number to access
 * @return 0 or 1
 */
uint8_t BigIntGetBit(const BigInt_t* n, unsigned int b);

/**
 * Sets bit b in n to v.
 * @param n Pointer to number
 * @param b Bit number to access
 * @param v 0 or 1
 */
uint8_t BigIntSetBit(BigInt_t* n, unsigned int b, uint8_t v);

#ifdef __cplusplus
}
#endif

#endif
