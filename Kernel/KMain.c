#include "KLib.h"
#include "FDT.h"

uint8_t Stack[4096];

void KMain(fdt_header *fdt)
{
    //* Execution reaches here
    //^ VGA pixel or letter visual
    // What we need here is drivers and other absolute basic functionality
    // Also be aware that a lot of this MIGHT have issues. I am unaware of how far the memory will be loaded.
    // File system is probably vital soon
        // File system will require some kind of generator function here (add that to Makefile when necessary)

    //* Successfully extracts FDT_Magic. Confirmed FDT structure
    if (fdt->magic != REVERSE_32(FDT_MAGIC))
        panic();
    fdt_parsed_list test;
    fdt_parse(fdt, &test);
    // Alright, so the FDT seems to work, we need to save all these important values in a struct for an FDT parser func

    //^ Find devices of interest for now.
        //^ VGA, MMIO, RESERVED, RAM, etc

    //^ Parse FDT
    //^ Set up basic (fatal error) interrupt vector. (panic() basically) (requires FDT interrupt controller locations)
        //^ Modify Boot.asm to set CSR values.
        //^ Add register functions to set/get CSR values.
    //^ Setup debug output (VGA or UART)
    
    
    return;
}