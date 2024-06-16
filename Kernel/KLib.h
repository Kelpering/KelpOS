#ifndef _KMAIN_H_
#define _KMAIN_H_

//* Typedefs

// UINT
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long uint64_t;

// INT
typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;
typedef signed long int64_t;

// MISC
typedef uint64_t size_t;
#define true 1
#define false 0
#define error -1
#define NULL 0

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

void kpanic(uint64_t error_code) __attribute__((noreturn));

//! All functions below this point are untested.

void *memset(void *str, int c, size_t n);

void *memcpy(void *dest, const void *src, size_t n);

int memcmp(const void *str1, const void *str2, size_t n);

#endif // _KMAIN_H_