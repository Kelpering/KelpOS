#include "Klib.h"
#include "Riscv.h"
#include "Fdt.h"
#include "Interrupt.h"
#include "Uart.h"
#include "Memory.h"
#include "Virtmem.h"

extern void _mpanic();
void kmain();

__attribute__((aligned (16))) volatile uint8_t kern_stack[4096];

void enter_c()
{
    volatile uint64_t temp_mstatus;
    asm volatile ("csrr %0, mstatus" : "=r" (temp_mstatus));
    // Set MPP for MRET
    temp_mstatus &= ~(3L<<11);          // Clear MPP (bits 11 and 12)
    temp_mstatus |=  (1L<<11);          // Set bit 11 (Sets MPP to S-Mode)
    // Set MIE for interrupts (SIE is disabled if MIE is 0)
    temp_mstatus |=  (1<<3);
    asm volatile ("csrw mstatus, %0" : : "r" (temp_mstatus));

    // Set exception counter (epc) to kmain
    volatile uint64_t temp_mepc = (uint64_t) kmain;
    asm volatile ("csrw mepc, %0" : : "r" (temp_mepc));

    // Disable paging
    satp_write(0);

    // Delegate all machine Interrupts and Exceptions to supervisor
    uint64_t temp_deleg = 0xFFFF;   // Delegate all Interrupts (16) and the first 16 exceptions (rest are reserved or unimportant)
    asm volatile ("csrw medeleg, %0" : : "r" (temp_deleg));
    asm volatile ("csrw mideleg, %0" : : "r" (temp_deleg));

    // Set Machine vector to be mpanic (in case of critical error in M-Mode)
    uint64_t temp_mtvec = (uint64_t) _mpanic;
    asm volatile ("csrw mtvec, %0" : : "r" (temp_mtvec));

    // Configure Physical Memory Protection (PMP) to allow supervisor access to all memory
    uint64_t temp_pmpaddr0 = 0x3FFFFFFFFFFFFFULL;   // Set pmpaddr0 to be the maximum holdable value (54 bits)
    uint64_t temp_pmpcfg0 = 0xF;                    // Set RWX along with access check TOR (effective address pmpaddr0 thru 0)
    asm volatile("csrw pmpaddr0, %0" : : "r" (temp_pmpaddr0));
    asm volatile("csrw pmpcfg0, %0" : : "r" (temp_pmpcfg0));

    // Restore MPP (set to S-Mode)
    // Restore mepc (Set to beginning of kmain)
    asm volatile("mret");
}


void kmain()
{
    init_fdt();
    init_interrupts();
    init_uart();
    init_memory();
    init_virtmem();

    uart_printf("\n");
    uart_printf("Booting KelpOS\n\n");

    uart_printf("-----------------------------------\n");
    uart_printf("Start of Kernel: %P\n", START_OF_KERNEL);
    uart_printf("End of Text:     %P\n", END_OF_TEXT);
    uart_printf("End of Kernel:   %P\n", END_OF_KERNEL);
    uart_printf("Start of Memory: %P\n", START_OF_MEM);
    uart_printf("End of Memory:   %P\n", END_OF_MEM);
    uart_printf("-----------------------------------\n\n");

    

    //* S-Mode conversions
    //* Interrupts
    //* Uart init
    //* Uart kpanic
    //* Uart interrupts (return sent characters to console to allow display)
    //* K Memory (page-grain alloc)
    //* K Memory (paging)
    //* Multitasking protections
    //? The kernel will allow interruptions while in S-Mode
    //? Instead of spinlocks (which function between harts) we will use just interrupt disable
    //? While in critical sections of code (such as UART, DISK, etc) we will disable interrupts for a period, then reenable
    //? Utilizing push/pop off will also prevent accidental overwrites or enables of interrupts when undesired.
    //? Implement that as the multitasking protection and we are golden.
    //^ Userland
        //^ Allocate Virtual memory for program (allocProg)
        //^ Userland test function (write to Virt Mem)
        //^ Userland interrupt vector
        // We will utilize disk at a later point to load programs.
        // All userland will be at this point is an executable with no syscall
        // Once we enter userland, we cannot leave (until later)
    //^ Syscall
        //^ SYS_test (just test if syscalls work, write a pointer?)
        //^ SYS_sbrk (expand or contract available memory to the program)
        //^ 
    //^ Disk
        //^ mkfs seperate program so the Kernel itself doesnt do that.
        //^ Buffer system (UNIX design principles book will be useful here)
        //^ File System
        //^ Userland Executables
    //^ Userland Continued
        //^ Load userland executable files into memory as program.
        //^ User.ld
        //^ Ulib
        //^ Malloc
    //^ Multitasking
        //^ Timer interrupts
        //^ Debug deadlocks, race conditions, general errors, and other fun :)
    //^ VGA


    uart_printf("\n\n");
    uart_panic("END OF KERNEL", "");
}