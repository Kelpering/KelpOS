#ifndef _KMAIN_H_
#define _KMAIN_H_

#include "intdef.h"

//* Endian macros

#define REVERSE_64(x) ((uint64_t)(\
    ((uint64_t)(x)&0xFF00000000000000) >> 56 | \
    ((uint64_t)(x)&0x00FF000000000000) >> 40 | \
    ((uint64_t)(x)&0x0000FF0000000000) >> 24 | \
    ((uint64_t)(x)&0x000000FF00000000) >> 8  | \
    ((uint64_t)(x)&0x00000000FF000000) << 8  | \
    ((uint64_t)(x)&0x0000000000FF0000) << 24 | \
    ((uint64_t)(x)&0x000000000000FF00) << 40 | \
    ((uint64_t)(x)&0x00000000000000FF) << 56   \
))

#define REVERSE_32(x) ((uint32_t)(\
    ((uint32_t)(x)&0xFF000000) >> 24 | \
    ((uint32_t)(x)&0x00FF0000) >> 8  | \
    ((uint32_t)(x)&0x0000FF00) << 8  | \
    ((uint32_t)(x)&0x000000FF) << 24   \
))

#define REVERSE_16(x) ((uint16_t)(\
    ((uint16_t)(x)&0xFF00) >> 8 | \
    ((uint16_t)(x)&0x00FF) << 8   \
))

//* Functions

void panic() __attribute__((noreturn));

#endif // _KMAIN_H_