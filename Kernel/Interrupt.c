#include "Interrupt.h"
#include "Fdt.h"
#include "Uart.h"

extern void interrupt_vec();
void init_plic_device(uint32_t irq);

void init_interrupts()
{
    //* Set interrupt_vec() to be the interrupt vector.
    // Clear first 2 bits to set mode to Direct.
    uint64_t temp_stvec = ((uint64_t) interrupt_vec & ~0x3);
    stvec_write(temp_stvec);

    //* Enable Supervisor external interrupts and disable timer interrupts
    uint64_t temp_sie = (sie_read() & ~0x2022) | 0x0200;
    sie_write(temp_sie);

    //* Enable PLIC (external interrupt controller)
    *(uint32_t*)(fdt_list.plic_addr + 0x201000) = 0;

    //* Enable individual IRQs from the PLIC
    init_plic_device(fdt_list.uart_irq);
    //^ more external devices

    //* Activate supervisor interrupts globally
    uint64_t temp_sstatus = sstatus_read();
    temp_sstatus |= (1 << 1);
    sstatus_write(temp_sstatus);

    return;
}

void interrupt_handler()
{
    uint64_t temp_scause = scause_read();
    uint64_t int_reason = temp_scause  & ~(1L<<63);
    uint32_t plic_irq;

    if ((temp_scause >> 63 & 1) == 1)
    {
        switch (temp_scause & ~(1L<<63))
        {
        //^ Timer interrupts for context switch
        case 9:    // PLIC
            plic_irq = claim_plic();

            if (plic_irq == fdt_list.uart_irq)
                uart_handler();
            //^ Add more conditionals as irq grows
            
            complete_plic(plic_irq);
            break;
        
        default:    // Unknown
            uart_panic("UNKNOWN INTERRUPT", "Int Cause: %d", (temp_scause & ~(1L<<63)));
            break;
        }
            
    }
    else
    {
        switch (int_reason)
        {
        case  9:    // S-Mode ecall
        case 11:    // U-Mode ecall
            uart_panic("ECALL", "Int Cause: %d", (int_reason));
            // Write sepc + 4, otherwise the call will repeat
            // sepc_write(sepc_read() + 4);
            break;
        case 12:
        case 13:
        case 15:
            uart_panic("PAGE FAULT", "12: Execute\n13: Read\n15: Write\nReason: %d", int_reason);
            break;

        default:
            // Modified epanic, in case of stack corruption. Display scause and err_code in registers.
            asm ("li x1, 0xDEAD");
            asm ("mv x2, %0" : : "r" (temp_scause));
            while (true)
                asm("wfi");
            break;
        }
    }
}

void init_plic_device(uint32_t irq)
{
    *(uint32_t*)(fdt_list.plic_addr + irq*4) = 1;
    *(uint32_t*)(fdt_list.plic_addr + 0x2080) = 1 << irq;
    return;
}

uint32_t claim_plic()
{   
    uint32_t irq = *(uint32_t*)(fdt_list.plic_addr+0x201004);
    return irq;
}

void complete_plic(uint32_t irq)
{
    *(uint32_t*)(fdt_list.plic_addr+0x201004) = irq;
    return;
}