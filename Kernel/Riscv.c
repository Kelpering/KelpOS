#include "Riscv.h"

uint32_t push_off_depth = 0;    // Depth of push_off/pop_off functions
bool_t prev_int;                // Previous interrupt status (before first push/pop).

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

void intr_on()
{
    sstatus_write(sstatus_read() | 0x2);
    return;
}

void intr_off()
{
    sstatus_write(sstatus_read() & ~0x2);
    return;
}

bool_t intr_get()
{
    return (sstatus_read() >> 1) & 0x1;
}

void push_off()
{
    // Save previous interrupt status.
    bool_t old = intr_get();

    // Disable interrupts. Save interrupts if this is the first push_off().
    intr_off();
    if (push_off_depth == 0)
        prev_int = old;
    push_off_depth++;

    return;
}

void pop_off()
{
    // Check for invalid pop_off states
    if (intr_get() == true)
        uart_panic("POP_OFF", "Interrupts enabled in pop_off()");
    if (push_off_depth < 1)
        uart_panic("POP_OFF", "push_off_depth impossible");

    // Enable interrupts only if we both reach the end of the push_off nest and were previously enabled.
    push_off_depth--;
    if (push_off_depth == 0 && prev_int)
        intr_on();
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