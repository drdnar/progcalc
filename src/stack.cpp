#include <stdlib.h>
#include <string.h>
#include "stack.h"
#include "forms/inlineoptimizations.h"


BigIntStack::BigIntStack(unsigned int max_size)
{
    if (max_size == 0) /* ? ? ? */
        max_size = 16;
    stack = (BigInt_t*)calloc(max_size, sizeof(BigInt_t));
    if (!stack)
        exit(1);
    maxSize = max_size;
    size = 0;
}


BigIntStack::~BigIntStack()
{
    free(stack);
}


BigIntStack& BigIntStack::operator=(const BigIntStack& source)
{
    if (this == &source)
        return *this;
    if (source.maxSize > maxSize)
        if (!(stack = (BigInt_t*)realloc(stack, sizeof(BigInt_t) * source.maxSize)))
            exit(1);
    maxSize = source.maxSize;
    size = source.size;
    memcpy(stack, source.stack, sizeof(BigInt_t) * size);
    return *this;
}


void BigIntStack::SerializeTo(void* target)
{
    *((unsigned int*)target) = maxSize;
    *((unsigned int*)target + 1) = size;
    memcpy((unsigned int*)target + 2, stack, sizeof(BigInt_t) * size);
}


void BigIntStack::DeserializeFrom(const void* source)
{
    auto newmax = *((const unsigned int*)source);
    if (newmax > maxSize)
        if (!(stack = (BigInt_t*)realloc(stack, sizeof(BigInt_t) * newmax)))
            exit(1);
    maxSize = newmax;
    size = *((const unsigned int*)source + 1);
    memcpy(stack, (const unsigned int*)source + 2, sizeof(BigInt_t) * size);
}


size_t BigIntStack::GetSerializedSize()
{
    return sizeof(maxSize) + sizeof(size) + sizeof(BigInt_t) * size;
}


void BigIntStack::Push(BigInt_t* number)
{
    if (size == maxSize)
    {
        if (maxSize == 1)
        {
            BigIntCopyFromTo(number, stack);
            return;
        }
        memmove(stack, stack + 1, (maxSize - 1) * sizeof(BigInt_t));
        BigIntCopyFromTo(number, stack + maxSize - 1);
        return;
    }
    BigIntCopyFromTo(number, stack + size++);
}


BigInt_t* BigIntStack::Peek()
{
    if (size == 0)
        return nullptr;
    return stack + size - 1;
}


BigInt_t* BigIntStack::Peek(unsigned int n)
{
    if (n >= size)
        return nullptr;
    return Peek() - n;
}


BigInt_t* BigIntStack::PeekFromBottom(unsigned int n)
{
    if (n >= size)
        return nullptr;
    return stack + n;
}


BigInt_t* BigIntStack::PopStalePointer()
{
    if (!size)
        return nullptr;
    return stack + --size;
}


bool BigIntStack::Pop(BigInt_t* destination)
{
    if (!size)
        return false;
    if (!destination)
        return false;
    BigIntCopyFromTo(stack + --size, destination);
    return true;
}


signed int BigIntStack::Resize(unsigned int newSize)
{
    unsigned int r = 0;
    if (newSize < size)
        r = size - newSize;
    BigInt_t* newarr = (BigInt_t*)realloc(stack, newSize * sizeof(BigInt_t));
    if (newarr)
        stack = newarr;
    else
        return -1;
    return r;
}


void BigIntStack::RotateUp()
{
    if (size <= 1)
        return;
    BigInt_t temp;
    BigIntCopyFromTo(stack + size - 1, &temp);
    memmove(stack + 1, stack, (size - 1) * sizeof(BigInt_t));
    BigIntCopyFromTo(&temp, stack);
}


void BigIntStack::RotateDown()
{
    if (size <= 1)
        return;
    BigInt_t temp;
    BigIntCopyFromTo(stack, &temp);
    memmove(stack, stack + 1, (size - 1) * sizeof(BigInt_t));
    BigIntCopyFromTo(&temp, stack + size - 1);
}


void BigIntStack::ExchangeTop()
{
    if (size <= 1)
        return;
    BigInt_t* top = Peek();
    BigInt_t temp;
    BigIntCopyFromTo(top, &temp);
    BigIntCopyFromTo(top - 1, top);
    BigIntCopyFromTo(&temp, top - 1);
}


bool BigIntStack::Get(unsigned int n, BigInt_t* destination)
{
    if (n >= size)
        return false;
    if (destination)
        BigIntCopyFromTo(Peek() - n, destination);
    return true;
}


bool BigIntStack::GetFromBottom(unsigned int n, BigInt_t* destination)
{
    if (n >= size)
        return false;
    if (destination)
        BigIntCopyFromTo(stack + n, destination);
    return true;
}


void BigIntStack::Delete(unsigned int n)
{
    if (n >= size)
    {
        size = 0;
        return;
    }
    size -= n;
}


void BigIntStack::DeleteFromBottom(unsigned int n)
{
    if (n >= size)
    {
        size = 0;
        return;
    }
    memmove(stack, stack + n, (size - n) * sizeof(BigInt_t));
    size -= n;
}
