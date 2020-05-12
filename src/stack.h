#ifndef BIGINTSTACK_H
#define BIGINTSTACK_H
#include "bigint.h"

typedef struct
{
    unsigned int MaxSize;
    unsigned int Size;
    BigInt_t* Stack;
} BigIntStack_t;

/**
 * Creates a new stack object.
 * @param maxSize Maximum number of entries to support in stack.
 * @return Pointer to newly allocated stack.
 */
BigIntStack_t* BigIntStack_ctor(unsigned int maxSize);

/**
 * Destroys a stack object.
 * @param self Pointer to stack to destory.
 */
void BigIntStack_dtor(BigIntStack_t* self);

/**
 * Checks if the stack is empty.
 * @param self Pointer to stack
 * @return True if the stack is empty.
 */
bool BigIntStack_IsEmpty(BigIntStack_t* self);

/**
 * Checks if the stack is full.
 * @param self Pointer to stack
 * @return True if the stack is full.
 */
bool BigIntStack_IsFull(BigIntStack_t* self);

/**
 * Returns the number of entries currently on the stack.
 * @param self Pointer to stack
 * @return Number of live entries
 */
unsigned int BigIntStack_GetSize(BigIntStack_t* self);

/**
 * Returns the maximum number of entries the stack can hold.
 * @param self Pointer to stack
 * @return Maximum number of possible entries
 */
unsigned int BigIntStack_GetMaxSize(BigIntStack_t* self);

/**
 * Clears the stack to empty.
 * @param self Pointer to stack
 */
void BigIntStack_Flush(BigIntStack_t* self);

/**
 * Pushes a number onto the stack.
 * If the stack is full, the bottommost entry in the stack is removed.
 * @param self Pointer to stack
 * @param number Pointer to number to push
 */
void BigIntStack_Push(BigIntStack_t* self, const BigInt_t* number);

/**
 * Returns a pointer to the number on top of the stack.
 * @param self Pointer to stack
 * @return Pointer to number on top of stack, or NULL if the stack is empty.
 */
BigInt_t* BigIntStack_Peek(BigIntStack_t* self);

/**
 * Removes a number from the stack.
 * @param self Pointer to stack
 * @param destination Pointer to location to write popped number.  This may be NULL.
 * @return Returns a now-stale pointer to the top of stack.  Potentially useful if you plan to immediately use the number.
 * Will return NULL if the stack is empty.
 */
BigInt_t* BigIntStack_Pop(BigIntStack_t* self, BigInt_t* destination);

/**
 * Resides the backing store of stack.
 * @param self Pointer to stack
 * @param newSize New maximum number of entries for stack
 * @return Returns zero if no live stack entires were deleted; otherwise, returns the number of lost stack entries.
 * Returns -1 on failure.
 */
signed int BigIntStack_Resize(BigIntStack_t* self, unsigned int newSize);

/**
 * Rotates the stack such that the second entry on the stack becomes the first, 
 * and the first becomes the bottom.
 * @param self Pointer to stack
 */
void BigIntStack_RotateUp(BigIntStack_t* self);

/**
 * Rotates the stack such that the bottom entry on the stack becomes the top,
 * and the top becomes the second.
 * @param self Pointer to stack
 */
void BigIntStack_RotateDown(BigIntStack_t* self);

/**
 * Swaps the first and second items on the stack.
 * @param self Pointer to stack
 */
void BigIntStack_Exchange(BigIntStack_t* self);

/**
 * Gets a number from the stack based on its index counting down from the top.
 * The topmost entry is index zero, the next is one, &c.
 * @param self Pointer to stack
 * @param n Entry number to fetch
 * @param destination Optional pointer to BigInt to copy number into
 * @return Pointer to entry, or NULL if no such entry.
 */
BigInt_t* BigIntStack_Get(BigIntStack_t* self, unsigned int n, BigInt_t* destination);

/**
 * Gets a number from the stack based on its index counting up from the bottom.
 * The bottommost entry is index zero, the next is one, &c.
 * @param self Pointer to stack
 * @param n Entry number to fetch
 * @param destination Optional pointer to BigInt to copy number into
 * @return Pointer to entry, or NULL if no such entry.
 */
BigInt_t* BigIntStack_GetFromBottom(BigIntStack_t* self, unsigned int n, BigInt_t* destination);

/**
 * Deletes a number of entries from the top of the stack.
 * @param self Pointer to stack
 * @param n Number of entries to remove
 */
void BigIntStack_Delete(BigIntStack_t* self, unsigned int n);

/**
 * Deletes a number of entries from the bottom of the stack.
 * @param self Pointer to stack
 * @param n Number of entries to remove
 */
void BigIntStack_DeleteFromBottom(BigIntStack_t* self, unsigned int n);

#endif
