#include "Riscv.h"

//* Template read/write pair code (replace [CSR] with appropriate csr)
// uint64_t [CSR]_read()
// {
//     volatile uint64_t value;
//     asm volatile ("csrr %0, [CSR]" : "=r" (value));
//     return value;
// }

// void [CSR]_write(uint64_t value)
// {
//     asm volatile ("csrw [CSR], %0" : : "r" (value));
//     return;
// }

uint64_t mstatus_read()
{
    volatile uint64_t value;
    asm volatile ("csrr %0, mstatus" : "=r" (value));
    return value;
}

void mstatus_write(uint64_t value)
{
    asm volatile ("csrw mstatus, %0" : : "r" (value));
    return;
}


uint64_t mtvec_read()
{
    volatile uint64_t value;
    asm volatile ("csrr %0, mtvec" : "=r" (value));
    return value;
}

void mtvec_write(uint64_t value)
{
    asm volatile ("csrw mtvec, %0" : : "r" (value));
    return;
}


uint64_t mie_read()
{
    volatile uint64_t value;
    asm volatile ("csrr %0, mie" : "=r" (value));
    return value;
}

void mie_write(uint64_t value)
{
    asm volatile ("csrw mie, %0" : : "r" (value));
    return;
}


uint64_t mip_read()
{
    volatile uint64_t value;
    asm volatile ("csrr %0, mip" : "=r" (value));
    return value;
}

void mip_write(uint64_t value)
{
    asm volatile ("csrw mip, %0" : : "r" (value));
    return;
}


uint64_t mcause_read()
{
    volatile uint64_t value;
    asm volatile ("csrr %0, mcause" : "=r" (value));
    return value;
}

void mcause_write(uint64_t value)
{
    asm volatile ("csrw mcause, %0" : : "r" (value));
    return;
}


uint64_t mepc_read()
{
    volatile uint64_t value;
    asm volatile ("csrr %0, mepc" : "=r" (value));
    return value;
}

void mepc_write(uint64_t value)
{
    asm volatile ("csrw mepc, %0" : : "r" (value));
    return;
}