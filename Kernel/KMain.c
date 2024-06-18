#include "KLib.h"
#include "FDT.h"
#include "Riscv.h"
#include "Interrupts.h"
#include "Uart.h"

__attribute__((aligned (16))) volatile uint8_t Stack[4096];

extern uint8_t END_OF_KERNEL[];

void kmain()
{
    //* Execution reaches here
    //^ VGA pixel or letter visual
    // What we need here is drivers and other absolute basic functionality
    // Also be aware that a lot of this MIGHT have issues. I am unaware of how far the memory will be loaded.
    // File system is probably vital soon
        // File system will require some kind of generator function here (add that to Makefile when necessary)

    // There might have to be TWO(2) interrupt handlers in the future. One for kernel, one for Userspace
    //^ Make this handle the important KERNEL interrupts 
    //* Exceptions (kpanic and print some debug info into a register)
    //! Software and Timer are disabled. Re-enable Timer during a future initialization step (multitasking or basic timer)
    init_interrupts();
    init_uart();

    // Harvested Start from either _start or Kernel.ld start location
    uart_printf("Start of Kernel: %p\nEnd of Kernel: %p", 0x80000000L, END_OF_KERNEL);

    uart_printf("\n\n");
    kpanic(0xDEAD00000E0F);
    return;
}