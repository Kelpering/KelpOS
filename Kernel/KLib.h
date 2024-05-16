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

//! All functions below this point are untested.

// Need to decide how exactly a "global" Parsed_FDT would work to remove these.
// Might need to modify the gp (global pointer) to be valid.
// Extra linker details required probably.
// It would make sense to add a global FDT struct to the KLib header (for access to any KLib enabled programs)
// Look into this further.
void shutdown(fdt_parsed_list Parsed_FDT) __attribute__((noreturn));

void reset(fdt_parsed_list Parsed_FDT) __attribute__((noreturn));

void *memset(void *str, int c, size_t n);

void *memcpy(void *dest, const void *src, size_t n);

int memcmp(const void *str1, const void *str2, size_t n);

#endif // _KMAIN_H_