#include "enumtostring.h"
#include <string.h>

unsigned int Forms::EnumToString::Read(void* target) const
{
    unsigned int out = 0;
    #ifdef _EZ80
    memcpy(&out, target, EnumVariableSize);
    #else
        #error "Uuuh, this depends on endianess."
    #endif
    return out;
}


void Forms::EnumToString::Write(void* target, unsigned int value) const
{
    #ifdef _EZ80
    memcpy(target, &value, EnumVariableSize);
    #else
        #error "Uuuh, this depends on endianess."
    #endif
}
