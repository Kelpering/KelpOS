#ifndef __RISCV_H__
#define __RISCV_H__

#include "Klib.h"

//* Used to prevent caching issues with virtual memory, a FENCE instruction.
void sfence_vma();

//* Sstatus (Supervisor Status)
uint64_t sstatus_read();
void sstatus_write(uint64_t value);

//* Stvec (Supervisor trap vector)
uint64_t stvec_read();
void stvec_write(uint64_t value);

//* Sie (Supervisor exception enable)
uint64_t sie_read();
void sie_write(uint64_t value);

//* Scause (Supervisor cause)
uint64_t scause_read();
void scause_write(uint64_t value);

//* Sepc (Supervisor exception PC)
uint64_t sepc_read();
void sepc_write(uint64_t value);

//* Satp (Supervisor Address Translation and Protection)
uint64_t satp_read();
void satp_write(uint64_t value);

#endif // __RISCV_H__