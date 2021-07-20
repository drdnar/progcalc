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
        __asm__("lddr"
            :
            : "l"((uint8_t*)source + count - 1), "e"((uint8_t*)destination + count - 1), "c"(count)
            : "a", "cc"
            );
}


/**
 * Same as <string.h> memmove, but the destination MUST be after (at a higher
 * memory address than) the source, and forces efficient inline eZ80 assembly.
 * @param destination Target address, e.g. &somearr[i + 2]
 * @param source Source address, e.g. &somearr[i]
 * @param count Number of bytes to move MUST BE LESS THAN 65536
 */
static inline void memmove_forward_short(void* destination, const void* source, size_t count)
{
    __asm__("ld\ta, c\n\t"
            "or\ta, b\n\t"
            "jr\tz, $ + 4\n\t"
            "lddr"
        :
        : "l"((uint8_t*)source + count - 1), "e"((uint8_t*)destination + count - 1), "c"(count)
        : "a", "cc"
        );
}


/**
 * Same as <string.h> memmove, but the destination MUST be after (at a higher
 * memory address than) the source, and forces efficient inline eZ80 assembly.
 * @param destination Target address, e.g. &somearr[i + 2]
 * @param source Source address, e.g. &somearr[i]
 * @param count Number of bytes to move MUST be nonzero
 */
static inline void memmove_forward_nonzero(void* destination, const void* source, size_t count)
{
    __asm__("lddr"
        :
        : "l"((uint8_t*)source + count - 1), "e"((uint8_t*)destination + count - 1), "c"(count)
        : "a", "cc"
        );
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
        __asm__("ldir"
            :
            : "l"(source), "e"(destination), "c"(count)
            : "a", "cc"
            );
}


/**
 * Same as <string.h> memmove, but the destination MUST be before (at a lower
 * memory address than) the source, and forces efficient inline eZ80 assembly.
 * @param destination Target address, e.g. &somearr[i - 2]
 * @param source Source address, e.g. &somearr[i]
 * @param count Number of bytes to move MUST BE LESS THAN 65536
 */
static inline void memmove_backward_short(void* destination, const void* source, size_t count)
{
    __asm__("ld\ta, c\n\t"
            "or\ta, b\n\t"
            "jr\tz, $ + 4\n\t"
            "ldir"
        :
        : "l"(source), "e"(destination), "c"(count)
        : "a", "cc"
        );
}


/**
 * Same as <string.h> memmove, but the destination MUST be before (at a lower
 * memory address than) the source, and forces efficient inline eZ80 assembly.
 * @param destination Target address, e.g. &somearr[i - 2]
 * @param source Source address, e.g. &somearr[i]
 * @param count Number of bytes to move MUST BE NONZERO
 */
static inline void memmove_backward_nonzero(void* destination, const void* source, size_t count)
{
    __asm__("ldir"
        :
        : "l"(source), "e"(destination), "c"(count)
        : "a", "cc"
        );
}


/**
 * Same as <string.h> memcpy.
 * @param destination Target address
 * @param source Source address
 * @param count Number of bytes to copy
 */
static inline void memcpy_inline(void* destination, const void* source, size_t count)
{
    if (count)
        __asm__("ldir"
            :
            : "l"(source), "e"(destination), "c"(count)
            : "a", "cc"
            );
}


/**
 * Same as <string.h> memcpy.
 * @param destination Target address
 * @param source Source address
 * @param count Number of bytes to copy MUST BE LESS THAN 65536
 */
static inline void memcpy_inline_short(void* destination, const void* source, size_t count)
{
    __asm__("ld\ta, c\n\t"
            "or\ta, b\n\t"
            "jr\tz, $ + 4\n\t"
            "ldir"
        :
        : "l"(source), "e"(destination), "c"(count)
        : "a", "cc"
        );
}


/**
 * Same as <string.h> memcpy.
 * @param destination Target address
 * @param source Source address
 * @param count Number of bytes to copy MUST be nonzero
 */
static inline void memcpy_inline_nonzero(void* destination, const void* source, size_t count)
{
    __asm__("ldir"
        :
        : "l"(source), "e"(destination), "c"(count)
        : "a", "cc"
        );
}


/**
 * Returns x + 2.
 */
static inline unsigned int _ADD2_(unsigned int x)
{
    __asm__("inc\thl\n\t"
            "inc\thl"
        : "=l"(x)
        : "l"(x)
    );
    return x;
}

/**
 * Returns x + 3.
 */
static inline unsigned int _SUB2_(unsigned int x)
{
    __asm__("dec\thl\n\t"
            "dec\thl"
        : "=l"(x)
        : "l"(x)
    );
    return x;
}

/**
 * Returns x - 2.
 */
static inline unsigned int _ADD3_(unsigned int x)
{
    __asm__("inc\thl\n\t"
            "inc\thl\n\t"
            "inc\thl"
        : "=l"(x)
        : "l"(x)
    );
    return x;
}

/**
 * Returns x - 3.
 */
static inline unsigned int _SUB3_(unsigned int x)
{
    __asm__("dec\thl\n\t"
            "dec\thl\n\t"
            "dec\thl"
        : "=l"(x)
        : "l"(x)
    );
    return x;
}

#else

/**
 * This is the same as memmove() in <string.h>.  On the eZ80, this forces a
 * slightly more efficient inline LDDR instruction.
 */
#define memmove_forward memmove

/**
 * This is the same as memmove() in <string.h>.  On the eZ80, this forces a
 * slightly more efficient inline LDDR instruction.
 */
#define memmove_forward_short memmove

/**
 * This is the same as memmove() in <string.h>.  On the eZ80, this forces a
 * slightly more efficient inline LDDR instruction.
 */
#define memmove_forward_nonzero memmove

/**
 * This is the same as memmove() in <string.h>.  On the eZ80, this forces a
 * slightly more efficient inline LDIR instruction.
 */
#define memmove_backward memmove

/**
 * This is the same as memmove() in <string.h>.  On the eZ80, this forces a
 * slightly more efficient inline LDIR instruction.
 */
#define memmove_backward_short memmove

/**
 * This is the same as memmove() in <string.h>.  On the eZ80, this forces a
 * slightly more efficient inline LDIR instruction.
 */
#define memmove_backward_nonzero memmove

/**
 * Same as <string.h> memcpy.
 */
#define memcpy_inline memcpy

/**
 * Same as <string.h> memcpy.
 */
#define memcpy_inline_short memcpy

/**
 * Same as <string.h> memcpy.
 */
#define memcpy_inline_nonzero memcpy

#define _ADD2_(x) (x + 2)
#define _ADD3_(x) (x + 3)
#define _SUB2_(x) (x - 2)
#define _SUB3_(x) (x - 3)

#endif
#endif /* EZ80_INLINES_H */
