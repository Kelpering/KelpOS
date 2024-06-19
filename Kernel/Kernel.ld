OUTPUT_ARCH("riscv")
ENTRY(_start)

SECTIONS
{
    /* Place .text (containing _start symbol) is at -kernel jump point (assumed here) */
    . = 0x80000000;
    PROVIDE(START_OF_KERNEL = .);
    
    .text : {
        *(.text .text.*)
        . = ALIGN(0x1000);
    }

    /* STexts do not need to be seperated from corresponding Texts */

    .rodata : {
        . = ALIGN(16);
        *(.srodata .srodata.*)
        . = ALIGN(16);
        *(.rodata .rodata.*)
    }

    .data : {
        . = ALIGN(16);
        *(.sdata .sdata.*)
        . = ALIGN(16);
        *(.data .data.*)
    }

    .bss : {
        . = ALIGN(16);
        *(.sbss .sbss.*)
        . = ALIGN(16);
        *(.bss .bss.*)
    }

    PROVIDE(END_OF_KERNEL = .);
}