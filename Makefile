OBJS = \
Kernel/Boot.o Kernel/Kmain.o Kernel/Riscv.o Kernel/Klib.o Kernel/Fdt.o Kernel/Interrupt.o \
Kernel/IntVec.o Kernel/Uart.o Kernel/Memory.o Kernel/Virtmem.o

QEMU = qemu-system-riscv64
GDB = riscv64-unknown-elf-gdb
CC = riscv64-unknown-elf-gcc
AS = riscv64-unknown-elf-as
LD = riscv64-unknown-elf-ld

QEMUFLAGS = -machine virt -bios none -kernel Kernel.elf -m 128M -smp 1 -serial mon:stdio -display sdl -device VGA -global virtio-mmio.force-legacy=false -drive file=fs.bin,if=none,format=raw,id=x0 -device virtio-blk-device,drive=x0,bus=virtio-mmio-bus.0
CCFLAGS = -g -c -Wall -Wpedantic -Werror -ffreestanding -fno-common -nostdlib -mno-relax -fno-stack-protector -fno-omit-frame-pointer -fno-pie -no-pie -O0 -mcmodel=medany
ASFLAGS = -g
LDFLAGS = -T Kernel/Kernel.ld
FSSIZE = 4096x8

.PHONY: all run debug dump clean purge

all: run clean

run: Kernel.elf
	$(QEMU) $(QEMUFLAGS)

debug: Kernel.elf
	gnome-terminal -- $(GDB) Kernel.elf
	$(QEMU) $(QEMUFLAGS) -S -gdb tcp::26000

dump: Kernel.elf
	$(QEMU) $(QEMUFLAGS) -machine dumpdtb=qemu.dtb
	dtc -o qemu.dts qemu.dtb

clean:
	rm -rf *.o *.elf Kernel/*.o

purge: clean
	rm -rf *.bin *.dtb *.dts

Kernel.elf: $(OBJS) fs.bin
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

fs.bin:
	touch fs.bin
	# Required command "dd" to fill the file to a max file size. The actual file does not expand during OS operation, so this is required.
	dd if=/dev/zero of=fs.bin bs=$(FSSIZE) count=1

%.o: %.c
	$(CC) $(CCFLAGS) $< -o $@

%.o: %.asm
	$(AS) $(ASFLAGS) $< -o $@
