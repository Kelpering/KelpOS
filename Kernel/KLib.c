#include "KLib.h"
#include "Uart.h"

void kpanic(uint64_t error_code)
{
    // Replace asm with better debug info later
    asm volatile("mv x1, %0" : : "r" (error_code));
    uart_printf("\nKPANIC = %X\n", error_code);
    while(1)
        asm("wfi");
}

void *memset(void *str, int c, size_t n)
{
    for (size_t i = 0; i < n; i++)
        ((uint8_t*)str)[i] = c;

    return str;
}

void *memcpy(void *dest, const void *src, size_t n)
{
    for(size_t i = 0; i < n; i++)
        ((uint8_t*)dest)[i] = ((uint8_t*)src)[i];

    return dest;
}

int memcmp(const void *str1, const void *str2, size_t n)
{
    int CMP = 0;
    for (size_t i = 0; i < n; i++)
    {
        CMP = ((uint8_t*)str1)[i] - ((uint8_t*)str2)[i];
        if (CMP != 0)
            break;
    }

    return CMP;
}