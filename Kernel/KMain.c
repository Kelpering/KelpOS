#include "KLib.h"
#include "FDT.h"
#include "Riscv.h"
#include "Interrupts.h"
#include "Uart.h"

volatile uint8_t Stack[4096];

void KMain()
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

    //^ Modify uart_print into uart_printf with variadic arguments
    //^ <stdarg.h> is freestanding, so we can use the variadic arguments in there for uart_printf.
    uart_print("\n");
    uart_print("UART initialized\n");
    uart_print("> ");


    kpanic(0xDEAD00000E0F);
    return;
}