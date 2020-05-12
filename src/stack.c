#include <stdlib.h>
#include <string.h>
#include "stack.h"

BigIntStack_t* BigIntStack_ctor(unsigned int maxSize)
{
    BigIntStack_t* this;
    if (maxSize == 0) /* ? ? ? */
        return NULL;
    this = malloc(sizeof(BigIntStack_t));
    if (!this)
        return this;
    if (!(this->Stack = calloc(maxSize, sizeof(BigInt_t))))
    {
        free(this);
        return NULL;
    }
    this->MaxSize = maxSize;
    this->Size = 0;
    return this;
}


void BigIntStack_dtor(BigIntStack_t* this)
{
    free(this);
}


bool BigIntStack_IsEmpty(BigIntStack_t* this)
{
    return this->Size == 0;
}


bool BigIntStack_IsFull(BigIntStack_t* this)
{
    return this->Size == this->MaxSize;
}


unsigned int BigIntStack_GetSize(BigIntStack_t* this)
{
    return this->Size;
}


unsigned int BigIntStack_GetMaxSize(BigIntStack_t* this)
{
    return this->MaxSize;
}


void BigIntStack_Flush(BigIntStack_t* this)
{
    this->Size = 0;
}


void BigIntStack_Push(BigIntStack_t* this, const BigInt_t* number)
{
    if (this->Size == this->MaxSize)
    {
        if (this->MaxSize == 1)
        {
            BigIntCopyFromTo(number, this->Stack);
            return;
        }
        memmove(this->Stack, this->Stack + 1, (this->MaxSize - 1) * sizeof(BigInt_t));
        BigIntCopyFromTo(number, this->Stack + this->MaxSize - 1);
        return;
    }
    BigIntCopyFromTo(number, this->Stack + this->Size++);
}


BigInt_t* BigIntStack_Peek(BigIntStack_t* this)
{
    if (this->Size == 0)
        return NULL;
    return this->Stack + this->Size - 1;
}


BigInt_t* BigIntStack_Pop(BigIntStack_t* this, BigInt_t* destination)
{
    if (this->Size == 0)
        return NULL;
    if (destination)
        BigIntCopyFromTo(this->Stack + this->Size - 1, destination);
    return this->Stack + --this->Size;
}


signed int BigIntStack_Resize(BigIntStack_t* this, unsigned int newSize)
{
    BigInt_t* new;
    unsigned int r = 0;
    if (newSize < this->Size)
        r = this->Size - newSize;
    new = realloc(this->Stack, newSize * sizeof(BigInt_t));
    if (new)
        this->Stack = new;
    else
        return -1;
    return r;
}


void BigIntStack_RotateUp(BigIntStack_t* this)
{
    BigInt_t temp;
    if (this->Size <= 1)
        return;
    BigIntStack_Pop(this, &temp);
    memmove(this->Stack + 1, this->Stack, (this->Size - 1) * sizeof(BigInt_t));
    BigIntCopyFromTo(&temp, this->Stack);
}


void BigIntStack_RotateDown(BigIntStack_t* this)
{
    BigInt_t temp;
    if (this->Size <= 1)
        return;
    BigIntCopyFromTo(this->Stack, &temp);
    memmove(this->Stack, this->Stack + 1, (this->Size - 1) * sizeof(BigInt_t));
    BigIntCopyFromTo(&temp, this->Stack + this->Size - 1);
}


void BigIntStack_Exchange(BigIntStack_t* this)
{
    BigInt_t temp;
    BigInt_t* top;
    if (this->Size <= 1)
        return;
    top = BigIntStack_Peek(this);
    BigIntCopyFromTo(top, &temp);
    BigIntCopyFromTo(top - 1, top);
    BigIntCopyFromTo(&temp, top);
}


BigInt_t* BigIntStack_Get(BigIntStack_t* this, unsigned int n, BigInt_t* destination)
{
    BigInt_t* item = BigIntStack_Peek(this) - n;
    if (n >= this->Size)
        return NULL;
    if (destination)
        BigIntCopyFromTo(item, destination);
    return item;
}


BigInt_t* BigIntStack_GetFromBottom(BigIntStack_t* this, unsigned int n, BigInt_t* destination)
{
    BigInt_t* item = this->Stack + n;
    if (n >= this->Size)
        return NULL;
    if (destination)
        BigIntCopyFromTo(item, destination);
    return item;
}


void BigIntStack_Delete(BigIntStack_t* this, unsigned int n)
{
    if (n >= this->Size)
    {
        this->Size = 0;
        return;
    }
    this->Size -= n;
}


void BigIntStack_DeleteFromBottom(BigIntStack_t* this, unsigned int n)
{
    if (n >= this->Size)
    {
        this->Size = 0;
        return;
    }
    memmove(this->Stack, this->Stack + n, (this->Size - n) * sizeof(BigInt_t));
    this->Size -= n;
}
