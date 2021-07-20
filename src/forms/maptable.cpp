#include "maptable.h"
#include "inlineoptimizations.h"

using namespace Forms;


const MapTableEntry* ConstMapTable::locate(const MapTableEntry* table, size_t table_size, unsigned int input)
{
    size_t mid = table_size / 2;
    const MapTableEntry* pivot_point = table + mid;
    unsigned int pivot = pivot_point->In;
    if (input == pivot)
        return pivot_point;
    else
    {
        if (table_size < 2)
            return nullptr;
        if (input < pivot)
            return locate(table, mid, input);
        else
            return locate(pivot_point + 1, table_size - mid - 1, input);
    }
}


bool ConstMapTable::map(const MapTableEntry* table, size_t table_size, unsigned int input, unsigned char output_size, AnyIntegralType* output)
{
    const MapTableEntry* item = locate(table, table_size, input);
    if (item)
    {
        if (output)
            memcpy(output, &item->Out, output_size);
        return true;
    }
    return false;
}


size_t ConstMapTable::IndexOf(unsigned int input) const
{
    const MapTableEntry* item = locate(List, Count, input);
    if (item)
        return item - List;
    return (size_t)-1;
}


AnyIntegralType ConstMapTable::Map(unsigned int input) const
{
    AnyIntegralType out { 0 };
    if (map(List, Count, input, OutputSize, &out))
        return out;
    else
        return DefaultValue;
}
