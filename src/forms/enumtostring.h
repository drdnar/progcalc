#ifndef ENUM_TO_STRING_H
#define ENUM_TO_STRING_H

#include "maptable.h"

namespace Forms
{

/**
 * Given an enum's value, returns a string.
 */
class EnumToString
{
    public:
        #ifndef ENUM_TO_STRING_DELETABLE
        /**
         * @param table Reference to table.
         * @param varsize Size, in bytes, of how big an instance of this enum is.
         */
        EnumToString(ConstMapTable& table, size_t varsize) : Values { &table }, EnumVariableSize { varsize } { }
        #else
        /**
         * @param table Reference to table.  No freeing will occur on destruct.
         * @param varsize Size, in bytes, of how big an instance of this enum is.
         */
        EnumToString(ConstMapTable& table) : Values { &table }, EnumVariableSize { varsize }, free_table_on_destruct { false } { }
        #endif
        #ifdef ENUM_TO_STRING_DELETABLE
        /**
         * @param Table Pointer to table.  This is freed on destruct.
         * @param varsize Size, in bytes, of how big an instance of this enum is.
         */
        EnumToString(ConstMapTable* Table) : Values { table }, EnumVariableSize { varsize }, free_table_on_destruct { true } { }
        ~EnumToString()
        {
            if (free_table_on_destruct)
                delete Values;
        }
        #endif
        /**
         * Converts an enum value into a string.
         */
        const char* Get(unsigned int input) const
        {
            return (const char*)Values->Map(input).CPtr;
        }
        const char* Get(void* variable) const
        {
            return Get(Read(variable));
        }
        /**
         * Reads from target, and zero-extends if necessary.
         */
        unsigned int Read(void* target) const;
        /**
         * Writes a new value to an instance of this enum.
         */
        void Write(void* target, unsigned int value) const;
        /**
         * Mapper from inputs to strings.
         * @note This is a bit of a hack but it's const so I guess it's fine?
         */
        const ConstMapTable* Values;
        /**
         * Number of bytes long the variable this processes is.
         */
        const size_t EnumVariableSize;
    private:
        #ifdef ENUM_TO_STRING_DELETABLE
        const bool free_table_on_destruct;
        #endif
};

} /* namespace Forms */

#endif /* ENUM_TO_STRING_H */