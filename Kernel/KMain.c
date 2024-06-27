#include "KLib.h"
#include "FDT.h"
#include "Riscv.h"
#include "Interrupts.h"
#include "Uart.h"
#include "Memory.h"
#include "Disk.h"

//! Stolen from xv6-riscv (mit-pdos github)
// qemu -machine virt is set up like this,
// based on qemu's hw/riscv/virt.c:
//
// 00001000 -- boot ROM, provided by qemu
// 02000000 -- CLINT
// 0C000000 -- PLIC
// 10000000 -- uart0 
// 10001000 -- virtio disk 
// 80000000 -- boot ROM jumps here in machine mode
//             -kernel loads the kernel here
// unused RAM after 80000000.





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
    init_disk();

    // Harvested Start from kernel.ld PROVIDE functions.
    uart_printf("\nStart of Kernel: %p\nEnd of Kernel: %p\n", START_OF_KERNEL, END_OF_KERNEL);

    //! The disk only works in multiples of 512 bytes (aka a sector).
    //! Rewrite disk_access to work with singular blocks of 512, or maybe more?
    uint8_t buffer[512] = {1};
    disk_access(buffer, sizeof(buffer), 0, DISK_WRITE);
    disk_access(buffer, sizeof(buffer), 0, DISK_READ);
    

    

    uart_printf("\n\n");
    kpanic(0xDEAD00000E0F);
    return;
}