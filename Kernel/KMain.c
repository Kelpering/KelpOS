#include "KLib.h"
#include "FDT.h"
#include "Riscv.h"
#include "Interrupts.h"
#include "Uart.h"
#include "Memory.h"

__attribute__((aligned (16))) volatile uint8_t Stack[4096];

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
    init_memory();

    // Harvested Start from either _start or Kernel.ld start location
    uart_printf("Start of Kernel: %p\nEnd of Kernel: %p\n", START_OF_KERNEL, END_OF_KERNEL);


    // Tests kfree, should return the same block both times.
    void* test = kalloc();
    uart_printf("\nKalloc result: \n[%p]", test);
    kfree(test);

    void* test2= kalloc();
    uart_printf("\nKalloc result: \n[%p]", test2);
    kfree(test2);
    

    uart_printf("\n\n");
    kpanic(0xDEAD00000E0F);
    return;
}