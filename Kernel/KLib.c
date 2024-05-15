#include "KLib.h"

void panic()
{
    // Replace asm with better debug info later
    asm("li x1, 0xCCCCCCCC");
    while(1);
}