# Allegedly, QEMU RISC-V virt assumes the kernel starts at 0x80000000
# .LD script puts _start at 0x80000000

.section .text
.global _start
_start:
    # ^ Add initializing code to begin Interrupt Control


    # Load Stack (array within KMain) into stack pointer
    la sp, Stack

    # Add size of Stack (4096) to sp because it grows downwards.
    li t0, 4096
    add sp, sp, t0
    
    # Call function KMain in KMain.c
    #// paramter 1 is the FDT pointer
    #// mv a0, a1
    call KMain
    
    # If KMain returns, spin infinitely
1:
    wfi
    j   1b
