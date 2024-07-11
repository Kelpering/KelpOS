# QEMU RISC-V virt assumes the kernel starts at 0x80000000
# .LD script puts _start at 0x80000000

.section .text
.global _start
.global _mpanic

_start:
    # Load the end of kern_stack (see Kmain.c) into the stack pointer.
    # Arithmetic can be performed on the label without overhead
    la  sp, (kern_stack + 4095)
    
    # Call function enter_c in KMain.c
    call enter_c
    
    # If enter_c returns, spin indefinitely
1:
    wfi
    j   1b

# For mtvec (in case we have some exception or interrupt in machine mode)
_mpanic:
    li  x1, 0xDEAD0000C0DE
1:
    wfi
    j   1b
