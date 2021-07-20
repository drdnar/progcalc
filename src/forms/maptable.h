#ifndef MAP_TABLE_H
#define MAP_TABLE_H
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#endif /* ARRAY_SIZE */

namespace Forms
{

/**
 * Generic type to hold any type of data.
 * Used in place of templates to reduce code duplication.
 */
union AnyIntegralType
{
    unsigned char UChar;
    unsigned short UShort;
    unsigned int UInt;
    signed char SChar;
    signed short SShort;
    signed int SInt;
    void* Ptr;
    const void* CPtr;
    void (*Func)();
};

/**
 * An entry in a table of mappings from an Input number to an Output number.
 */
struct MapTableEntry final
{
    const unsigned int In;
    const AnyIntegralType Out;
};


/**
 * Maps an input value to an output value.
 * 
 * Lookups are performed through a binary search, so entires MUST be sorted in
 * ascending order.
 */
class ConstMapTable final
{
    public:
        ConstMapTable(AnyIntegralType Default, unsigned char Output_size, size_t count, const MapTableEntry* list)
         : DefaultValue { Default }, OutputSize { Output_size }, Count { count },  List { list }
         { }
        /**
         * Attempts to map an input to an output.
         * If a mapping is found, then output is overwritten.
         * @param input Value to search for
         * @param output Pointer to variable to receive output.  May be nullptr.
         * @return True if the input is successfully found, false otherwise.
         */
        bool TryMap(unsigned int input, AnyIntegralType* output) const
            { return map(List, Count, input, OutputSize, output); }
        /**
         * Looks up an input.  If no matching input is found, then the default value is returned.
         */
        AnyIntegralType Map(unsigned int input) const;
        /**
         * Looks for the index of an input value.
         * @return Returns -1 on failure.
         */
        size_t IndexOf(unsigned int input) const;
        /**
         * Default value to return if a mapping cannot be found.
         */
        const AnyIntegralType DefaultValue;
        /**
         * Number of bytes in output data type.
         */
        const unsigned char OutputSize;
        /**
         * Number of values in table.
         */
        const size_t Count;
        /**
         * List of mappings in table.
         */
        const MapTableEntry* List;
    private:
        /**
         * Performs a binary search for a mapping in a table.
         * @param table Pointer to table base to start search.
         * @param table_size Number of entries in the table.
         * @param input Value to search for.  
         * @return true if a mapping was found, false if not.
         * @internal This abuses the fact that "this" is a pointer and pointers are
         * convertable to arrays.
         */
        static const MapTableEntry* locate(const MapTableEntry* table, size_t table_size, unsigned int input);
        /**
         * Performs a binary search for a mapping in a table.
         * @param table Pointer to table base to start search.
         * @param table_size Number of entries in the table.
         * @param input Value to search for.  
         * @param output Pointer to variable to hold output.
         * If a mapping is found, it is overwritten with the mapped value.
         * If no mapping is found, then it is unchanged.
         * @param output_size Size of output data type.  Needed for overwriting correct number of bytes.
         * @return true if a mapping was found, false if not.
         * @internal This abuses the fact that "this" is a pointer and pointers are
         * convertable to arrays.
         */
        static bool map(const MapTableEntry* table, size_t table_size, unsigned int input, unsigned char output_size, AnyIntegralType* output);
};

#define MAP_TABLE(DEFAULT, TYPE, ARRAY) { AnyIntegralType { DEFAULT }, sizeof(TYPE), ARRAY_SIZE(ARRAY), ARRAY }

} /* namespace Forms */

#endif /* MAP_TABLE_H */
