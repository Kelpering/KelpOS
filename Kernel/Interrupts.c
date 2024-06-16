#include "Interrupts.h"
#include "FDT.h"
#include "Uart.h"

extern void interrupt_vec();

void init_interrupts()
{
    //* Set all interrupts & exceptions to trigger int_handler();
    uint64_t temp_mtvec = ((uint64_t) interrupt_vec & ~0) | 0;
    mtvec_write(temp_mtvec);

    // * Allow Machine external interrupts, disable all others.
    uint64_t temp_mie = 0x0800;
    mie_write(temp_mie);

    //! Untested, need to confirm that this works.
    //* Enable PLIC interrupts from the PLIC
    *(uint32_t*)(PLIC_ADDR + 0X200000) = 0;

    //* Enable individual IRQ from the PLIC
    init_plic_device(UART_IRQ);

    //* Enable global interrupts
    uint64_t temp_mstatus = mstatus_read();
    temp_mstatus = temp_mstatus | (1 << 3);
    mstatus_write(temp_mstatus);

    return;
}

void interrupt_handler(int_regs* regs)
{
    uint64_t temp_mcause = mcause_read();
    uint64_t mcause_code = temp_mcause & ~(1L<<63);

    //? Exception interrupts (including ecall)
    if ((temp_mcause >> 63 & 1) != 1)
    {
        switch (mcause_code)
        {
        case  8:    // U ECALL
        case  9:    // S ECALL
        case 11:    // M ECALL
            //* The return PC will overlap the instruction that caused this interrupt, so we must increment the epc by one instruction.
            mepc_write(mepc_read()+4);
            break;
        
        default:
            //* Unhandled interrupt stops here. Modified kpanic() in case of stack corruption.
            asm ("li x1, 0xDEAD");
            asm ("mv x2, %0" : : "r" (temp_mcause));
            while(true)
                asm("wfi");
            break;
        }
    }
    //? Device interrupts (PLIC)
    else    
    {
        //! This handles only external (PLIC) interrupts. Add timer interrupts at a later date.
        if (mcause_code != 11)
            kpanic(0xDEAD);

        uint32_t plic_irq = claim_plic();

        switch (plic_irq)
        {
        case UART_IRQ:
            uart_handler();
            break;
        
        default:
            //^ Add new devices to plic
            break;
        }

        complete_plic(plic_irq);
    }

    return;
}