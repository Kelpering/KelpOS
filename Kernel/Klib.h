#ifndef __KLIB_H__
#define __KLIB_H__

//* Typedefs
// INT
typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;
typedef signed long int64_t;

// UINT
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

// MISC
typedef uint64_t size_t;
#define true 1
#define false 0
#define error -1
#define NULL 0

//* Linker provided variables
extern uint8_t START_OF_KERNEL[];
extern uint8_t END_OF_TEXT[];
extern uint8_t END_OF_KERNEL[];

//* Macros
static inline __attribute__((always_inline, noreturn)) void epanic(uint64_t err_code) 
{
    asm volatile ("mv x1, %0" : : "r" (err_code));
    while (true)
        asm volatile("wfi");
}

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



/// @brief A more detailed panic that prints out key values and freezes the OS.
/// @note Assumes that uart has been initialized and C runtime is uncorrupted.
/// @warning Separated from Uart.h to allow for more universal access, might be invalid to use in certain cases.
/// @param err The error itself
/// @param fmt A secondary printf style fmt string to print extra info as needed.
void uart_panic(char *err, char *fmt, ...) __attribute__((noreturn));

void *memset(void *ptr, uint8_t byte, size_t n);
void *memcpy(void *dest, const void *src, size_t n);
int memcmp(const void *str1, const void *str2, size_t n);


#endif //__KLIB_H__