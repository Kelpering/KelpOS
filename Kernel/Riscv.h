#ifndef _RISCV_H_
#define _RSICV_H_

#include "KLib.h"

// Read & Write functions for specific CSR's as necessary
// Add small description above R/W pair for the CSR

//* MStatus (Machine Status)
//^ Configure MStatus within Boot.asm OR Configure MStatus within KMain.c
uint64_t mstatus_read();
void mstatus_write(uint64_t Value);

//* Mtvec (Machine Trap Vector)
uint64_t mtvec_read();
void mtvec_write(uint64_t value);

//* Mie (Machine Interrupt Enable)
uint64_t mie_read();
void mie_write(uint64_t value);

//* Mip (Machine Interrupt Pending)
uint64_t mip_read();
void mip_write(uint64_t value);

//* MCause (Machine Cause)
uint64_t mcause_read();
void mcause_write(uint64_t value);

//* MEpc (Machine Error PC)
uint64_t mepc_read();
void mepc_write(uint64_t value);


#endif // _RISC_H_