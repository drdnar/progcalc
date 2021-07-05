#ifndef EZ80_INLINES_H
#define EZ80_INLINES_H

#include <string.h>

#ifdef _EZ80

/**
 * Same as <string.h> memmove, but the destination MUST be after (at a higher
 * memory address than) the source, and forces efficient inline eZ80 assembly.
 * @param destination Target address, e.g. &somearr[i + 2]
 * @param source Source address, e.g. &somearr[i]
 * @param count Number of bytes to move
 */
static inline void memmove_forward(void* destination, const void* source, size_t count)
{
    if (count)
        __asm__("lddr":: "l"(source), "e"(destination), "c"(count));
}

/**
 * Same as <string.h> memmove, but the destination MUST be before (at a lower
 * memory address than) the source, and forces efficient inline eZ80 assembly.
 * @param destination Target address, e.g. &somearr[i - 2]
 * @param source Source address, e.g. &somearr[i]
 * @param count Number of bytes to move
 */
static inline void memmove_backward(void* destination, const void* source, size_t count)
{
    if (count)
        __asm__("ldir":: "l"(source), "e"(destination), "c"(count));
}

#else

/**
 * This is the same as memmove() in <string.h>.  On the eZ80, this forces a
 * slightly more efficient inline LDDR instruction.
 */
#define memmove_forward memmove

/**
 * This is the same as memmove() in <string.h>.  On the eZ80, this forces a
 * slightly more efficient inline LDIR instruction.
 */
#define memmove_backward memmove

#endif
#endif /* EZ80_INLINES_H */
