#include "Klib.h"

void *memset(void *ptr, uint8_t byte, size_t n)
{
    for (size_t i = 0; i < n; i++)
        ((uint8_t*)ptr)[i] = byte;

    return ptr;
}

void *memcpy(void *dest, const void *src, size_t n)
{
    for (size_t i = 0; i < n; i++)
        ((uint8_t*) dest)[i] = ((uint8_t*) src)[i];
        
    return dest;
}

int memcmp(const void *str1, const void *str2, size_t n)
{
    int cmp = 0;
    for (size_t i = 0; i < n; i++)
    {
        cmp = ((uint8_t*) str1)[i] - ((uint8_t*) str2)[i];
        if (cmp != 0)
            break;
    }

    return cmp;
}