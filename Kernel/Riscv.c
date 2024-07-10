#include "Riscv.h"

void sfence_vma()
{
    asm volatile("sfence.vma x0, x0");
    return;
}

uint64_t sstatus_read()
{
    volatile uint64_t value;
    asm volatile ("csrr %0, sstatus" : "=r" (value));
    return value;
}

void sstatus_write(uint64_t value)
{
    asm volatile("csrw sstatus, %0" : : "r" (value));
    return;
}


uint64_t stvec_read()
{
    volatile uint64_t value;
    asm volatile ("csrr %0, stvec" : "=r" (value));
    return value;
}

void stvec_write(uint64_t value)
{
    asm volatile("csrw stvec, %0" : : "r" (value));
    return;
}


uint64_t sie_read()
{
    volatile uint64_t value;
    asm volatile ("csrr %0, sie" : "=r" (value));
    return value;
}

void sie_write(uint64_t value)
{
    asm volatile("csrw sie, %0" : : "r" (value));
    return;
}


uint64_t scause_read()
{
    volatile uint64_t value;
    asm volatile ("csrr %0, scause" : "=r" (value));
    return value;
}

void scause_write(uint64_t value)
{
    asm volatile("csrw scause, %0" : : "r" (value));
    return;
}


uint64_t sepc_read()
{
    volatile uint64_t value;
    asm volatile ("csrr %0, sepc" : "=r" (value));
    return value;
}

void sepc_write(uint64_t value)
{
    asm volatile("csrw sepc, %0" : : "r" (value));
    return;
}


uint64_t satp_read()
{
    volatile uint64_t value;
    asm volatile ("csrr %0, satp" : "=r" (value));
    return value;
}

void satp_write(uint64_t value)
{
    asm volatile("csrw satp, %0" : : "r" (value));
    return;
}