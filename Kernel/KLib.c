#include "KLib.h"

void panic()
{
    // Replace asm with better debug info later
    asm("li x1, 0x10000000DEADBEEF");
    while(1)
        asm("wfi");
}

// void shutdown(fdt_parsed_list Parsed_FDT)
// {
//     *Parsed_FDT.power_addr = Parsed_FDT.power_value;
//     while (true);
// }

// void reset(fdt_parsed_list Parsed_FDT)
// {
//     *Parsed_FDT.reboot_addr = Parsed_FDT.reboot_value;
//     while (true);
// }

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