#include "KLib.h"

void panic()
{
    // Replace asm with better debug info later
    asm("li x1, 0x10000000DEADBEEF");
    while(1)
        asm("wfi");
}