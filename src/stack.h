#ifndef BIGINTSTACK_H
#define BIGINTSTACK_H
#include "bigint.h"


/**
 * Since there's no STL, this is a custom implementation of a stack specifically
 * for BigInt_t.
 * 
 * This was ported from C, so it's not very C++-like.
 */
class BigIntStack final
{
    public:
        /**
         * @param max_size Maximum number of entries to support in stack.
         */
        BigIntStack(unsigned int max_size);
        ~BigIntStack();
        /**
         * Assignment operator.
         */
        BigIntStack& operator=(const BigIntStack& source);
        /**
         * Stores the stack.  Only the portion of the stack with valid data are
         * written.
         */
        void SerializeTo(void* target);
        /**
         * Loads a stack that was previously serialized.
         */
        void DeserializeFrom(const void* target);
        /**
         * Computes the space required by SerializeTo().
         */
        size_t GetSerializedSize();
        /**
         * Checks if the stack is empty.
         * @return True if the stack is empty.
         */
        bool IsEmpty() const { return size == 0; }
        /**
         * Checks if the stack is full.
         * @return True if the stack is full.
         */
        bool IsFull() const { return size == maxSize; }
        /**
         * Returns the number of entries currently on the stack.
         * @return Number of live entries
         */
        unsigned int GetSize() const { return size; }
        /**
         * Returns the maximum number of entries the stack can hold.
         * @return Maximum number of possible entries
         */
        unsigned int GetMaxSize() const { return maxSize; }
        /**
         * Clears the stack to empty.
         * @param self Pointer to stack
         */
        void Flush() { size = 0; }
        /**
         * Pushes a number onto the stack.
         * If the stack is full, the bottommost entry in the stack is removed.
         * @param number Pointer to number to push
         */
        void Push(BigInt_t* number);
        /**
         * Returns a pointer to the number on top of the stack.
         * @return Pointer to number on top of stack, or NULL if the stack is empty.
         */
        BigInt_t* Peek();
        /**
         * Gets a number from the stack based on its index counting down from the top.
         * The topmost entry is index zero, the next is one, &c.
         * @param n Entry number to fetch
         * @return Pointer to entry, or NULL if no such entry.
         */
        BigInt_t* Peek(unsigned int n);
        /**
         * Gets a number from the stack based on its index counting up from the bottom.
         * The bottommost entry is index zero, the next is one, &c.
         * @param n Entry number to fetch
         * @return Pointer to entry, or NULL if no such entry.
         */
        BigInt_t* PeekFromBottom(unsigned int n);
        /**
         * Removes a number from the stack.
         * @return Returns a now-stale pointer to the top of stack.  Potentially useful if you plan to immediately use the number.
         * Will return NULL if the stack is empty.
         */
        BigInt_t* PopStalePointer();
        /**
         * Removes a number from the stack.
         * @param destination Pointer to location to write popped number.
         * @return Returns false if the stack is empty.
         */
        bool Pop(BigInt_t* destination);
        /**
         * Resizes the backing store of stack.
         * @param newSize New maximum number of entries for stack
         * @return Returns zero if no live stack entires were deleted; otherwise, returns the number of lost stack entries.
         * Returns -1 on failure.
         */
        signed int Resize(unsigned int newSize);
        /**
         * Rotates the stack such that the second entry on the stack becomes the first, 
         * and the first becomes the bottom.
         */
        void RotateUp();
        /**
         * Rotates the stack such that the bottom entry on the stack becomes the top,
         * and the top becomes the second.
         */
        void RotateDown();
        /**
         * Swaps the first and second items on the stack.
         */
        void ExchangeTop();
        /**
         * Gets a number from the stack based on its index counting down from the top.
         * The topmost entry is index zero, the next is one, &c.
         * @param n Entry number to fetch
         * @param destination Pointer to BigInt to copy number into
         * @return Returns false if no such stack entry exists.
         */
        bool Get(unsigned int n, BigInt_t* destination);
        /**
         * Gets a number from the stack based on its index counting up from the bottom.
         * The bottommost entry is index zero, the next is one, &c.
         * @param n Entry number to fetch
         * @param destination Pointer to BigInt to copy number into
         * @return Returns false if no such stack entry exists.
         */
        bool GetFromBottom(unsigned int n, BigInt_t* destination);
        /**
         * Deletes a number of entries from the top of the stack.
         * @param n Number of entries to remove
         */
        void Delete(unsigned int);
        /**
         * Deletes a number of entries from the bottom of the stack.
         * @param n Number of entries to remove
         */
        void DeleteFromBottom(unsigned int n);
    private:
        unsigned int maxSize;
        unsigned int size;
        BigInt_t* stack;
};


#endif /* BIGINTSTACK_H */
