#include <stdlib.h>
#include <string.h>
#include "stack.h"

BigIntStack_t* BigIntStack_ctor(unsigned int maxSize)
{
    BigIntStack_t* self;
    if (maxSize == 0) /* ? ? ? */
        return nullptr;
    self = (BigIntStack_t*)malloc(sizeof(BigIntStack_t));
    if (!self)
        return self;
    if (!(self->Stack = (BigInt_t*)calloc(maxSize, sizeof(BigInt_t))))
    {
        free(self);
        return nullptr;
    }
    self->MaxSize = maxSize;
    self->Size = 0;
    return self;
}


void BigIntStack_dtor(BigIntStack_t* self)
{
    free(self);
}


bool BigIntStack_IsEmpty(BigIntStack_t* self)
{
    return self->Size == 0;
}


bool BigIntStack_IsFull(BigIntStack_t* self)
{
    return self->Size == self->MaxSize;
}


unsigned int BigIntStack_GetSize(BigIntStack_t* self)
{
    return self->Size;
}


unsigned int BigIntStack_GetMaxSize(BigIntStack_t* self)
{
    return self->MaxSize;
}


void BigIntStack_Flush(BigIntStack_t* self)
{
    self->Size = 0;
}


void BigIntStack_Push(BigIntStack_t* self, const BigInt_t* number)
{
    if (self->Size == self->MaxSize)
    {
        if (self->MaxSize == 1)
        {
            BigIntCopyFromTo(number, self->Stack);
            return;
        }
        memmove(self->Stack, self->Stack + 1, (self->MaxSize - 1) * sizeof(BigInt_t));
        BigIntCopyFromTo(number, self->Stack + self->MaxSize - 1);
        return;
    }
    BigIntCopyFromTo(number, self->Stack + self->Size++);
}


BigInt_t* BigIntStack_Peek(BigIntStack_t* self)
{
    if (self->Size == 0)
        return nullptr;
    return self->Stack + self->Size - 1;
}


BigInt_t* BigIntStack_Pop(BigIntStack_t* self, BigInt_t* destination)
{
    if (self->Size == 0)
        return nullptr;
    if (destination)
        BigIntCopyFromTo(self->Stack + self->Size - 1, destination);
    return self->Stack + --self->Size;
}


signed int BigIntStack_Resize(BigIntStack_t* self, unsigned int newSize)
{
    BigInt_t* newarr;
    unsigned int r = 0;
    if (newSize < self->Size)
        r = self->Size - newSize;
    newarr = (BigInt_t*)realloc(self->Stack, newSize * sizeof(BigInt_t));
    if (newarr)
        self->Stack = newarr;
    else
        return -1;
    return r;
}


void BigIntStack_RotateUp(BigIntStack_t* self)
{
    BigInt_t temp;
    if (self->Size <= 1)
        return;
    BigIntCopyFromTo(self->Stack + self->Size - 1, &temp);
    memmove(self->Stack + 1, self->Stack, (self->Size - 1) * sizeof(BigInt_t));
    BigIntCopyFromTo(&temp, self->Stack);
}


void BigIntStack_RotateDown(BigIntStack_t* self)
{
    BigInt_t temp;
    if (self->Size <= 1)
        return;
    BigIntCopyFromTo(self->Stack, &temp);
    memmove(self->Stack, self->Stack + 1, (self->Size - 1) * sizeof(BigInt_t));
    BigIntCopyFromTo(&temp, self->Stack + self->Size - 1);
}


void BigIntStack_Exchange(BigIntStack_t* self)
{
    BigInt_t temp;
    BigInt_t* top;
    if (self->Size <= 1)
        return;
    top = BigIntStack_Peek(self);
    BigIntCopyFromTo(top, &temp);
    BigIntCopyFromTo(top - 1, top);
    BigIntCopyFromTo(&temp, top - 1);
}


BigInt_t* BigIntStack_Get(BigIntStack_t* self, unsigned int n, BigInt_t* destination)
{
    BigInt_t* item = BigIntStack_Peek(self) - n;
    if (n >= self->Size)
        return nullptr;
    if (destination)
        BigIntCopyFromTo(item, destination);
    return item;
}


BigInt_t* BigIntStack_GetFromBottom(BigIntStack_t* self, unsigned int n, BigInt_t* destination)
{
    BigInt_t* item = self->Stack + n;
    if (n >= self->Size)
        return nullptr;
    if (destination)
        BigIntCopyFromTo(item, destination);
    return item;
}


void BigIntStack_Delete(BigIntStack_t* self, unsigned int n)
{
    if (n >= self->Size)
    {
        self->Size = 0;
        return;
    }
    self->Size -= n;
}


void BigIntStack_DeleteFromBottom(BigIntStack_t* self, unsigned int n)
{
    if (n >= self->Size)
    {
        self->Size = 0;
        return;
    }
    memmove(self->Stack, self->Stack + n, (self->Size - n) * sizeof(BigInt_t));
    self->Size -= n;
}
