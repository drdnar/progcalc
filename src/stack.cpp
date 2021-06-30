#include <stdlib.h>
#include <string.h>
#include "stack.h"
#include "forms/memcpy.h"
#include <new>


#include <graphx.h>
#include <tice.h>

BigIntStack::BigIntStack(unsigned int max_size)
{
    if (max_size == 0) /* ? ? ? */
        max_size = 16;
    stack = (BigInt_t*)calloc(max_size, sizeof(BigInt_t));
/** TODO: Get angry and abort.
    if (!stack)
        throw std::bad_alloc();
*/
/*    if (!stack)
    {
        gfx_SetTextBGColor(12);
        gfx_PrintStringXY("  AAAARGH!  ", 1, 1);
        gfx_PrintUInt(max_size, 3);
        //do ; while (!os_GetCSC());
        max_size = 8;
        stack = (BigInt_t*)calloc(max_size, sizeof(BigInt_t));
        if (!stack)
        {
            gfx_PrintString("  ? ? ?  ");
            stack = (BigInt_t*)malloc(max_size * sizeof(BigInt_t));
            if (!stack)
                gfx_PrintString("Worked.");
        }
    }*/

    maxSize = max_size;
    size = 0;
}


BigIntStack::~BigIntStack()
{
    free(stack);
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
        memmove_backward(stack, stack + 1, (maxSize - 1) * sizeof(BigInt_t));
        BigIntCopyFromTo(number, stack + maxSize - 1);
        return;
    }
    BigIntCopyFromTo(number, stack + size++);
}


BigInt_t* BigIntStack::Peek(void)
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


void BigIntStack::RotateUp(void)
{
    if (size <= 1)
        return;
    BigInt_t temp;
    BigIntCopyFromTo(stack + size - 1, &temp);
    memmove_forward(stack + 1, stack, (size - 1) * sizeof(BigInt_t));
    BigIntCopyFromTo(&temp, stack);
}


void BigIntStack::RotateDown(void)
{
    if (size <= 1)
        return;
    BigInt_t temp;
    BigIntCopyFromTo(stack, &temp);
    memmove_backward(stack, stack + 1, (size - 1) * sizeof(BigInt_t));
    BigIntCopyFromTo(&temp, stack + size - 1);
}


void BigIntStack::ExchangeTop(void)
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
    memmove_backward(stack, stack + n, (size - n) * sizeof(BigInt_t));
    size -= n;
}
