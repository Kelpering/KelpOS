
OBJS = Kernel/Boot.o Kernel/KMain.o Kernel/KLib.o Kernel/FDT.o Kernel/Riscv.o Kernel/Interrupts.o Kernel/InterruptVec.o Kernel/Uart.o

QEMU = qemu-system-riscv64
GDB = riscv64-unknown-elf-gdb
CC = riscv64-unknown-elf-gcc
AS = riscv64-unknown-elf-as
LD = riscv64-unknown-elf-ld
QEMUFLAGS = -machine virt -bios none -kernel Kernel.elf -m 128M -smp 1 -serial mon:stdio -display sdl -device VGA
CCFLAGS = -g -c -Wall -Wpedantic -Werror -ffreestanding -fno-common -nostdlib -mno-relax -fno-stack-protector -fno-omit-frame-pointer -fno-pie -no-pie -O0 -mcmodel=medany
ASFLAGS = -g
LDFLAGS = -T Kernel/Kernel.ld

.PHONY: all run debug clean

all: run clean

run: Kernel.elf
	$(QEMU) $(QEMUFLAGS)

debug: Kernel.elf
	gnome-terminal -- $(GDB) Kernel.elf -ex "target remote localhost:26000" -ex "set disassemble-next-line on"
	$(QEMU) $(QEMUFLAGS) -S -gdb tcp::26000

clean:
	rm -rf *.o *.elf Kernel/*.o

Kernel.elf: $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

%.o: %.c
	$(CC) $(CCFLAGS) $< -o $@

%.o: %.asm
	$(AS) $(ASFLAGS) $< -o $@
