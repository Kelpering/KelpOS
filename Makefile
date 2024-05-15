
OBJS = Kernel/Boot.o Kernel/KMain.o Kernel/KLib.o

QEMU = qemu-system-riscv64
GDB = riscv64-unknown-elf-gdb
CC = riscv64-unknown-elf-gcc
AS = riscv64-unknown-elf-as
LD = riscv64-unknown-elf-ld
QEMUFLAGS = -machine virt -bios none -kernel Kernel.elf -m 128M -smp 1 -serial mon:stdio -display sdl -device VGA
CCFLAGS = -g -c -Wall -Wpedantic -ffreestanding -fno-common -nostdlib -mno-relax -fno-stack-protector -fno-omit-frame-pointer -fno-pie -no-pie -O0
ASFLAGS = -g
LDFLAGS = -T Kernel/Kernel.ld

.PHONY: all run debug clean

all: run clean

run: Kernel.elf
	$(QEMU) $(QEMUFLAGS)

debug: Kernel.elf
	gnome-terminal -- $(GDB) Kernel.elf -ex="target remote localhost:26000"
	$(QEMU) $(QEMUFLAGS) -S -gdb tcp::26000

clean:
	rm -rf *.o *.elf Kernel/*.o

Kernel.elf: $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

%.o: %.c
	$(CC) $(CCFLAGS) $< -o $@

%.o: %.asm
	$(AS) $(ASFLAGS) $< -o $@


# clean:
# 	- rm -rf *.o *.oasm *.elf *.iso ./IsoGen 
# 	- rm -rf Kernel/*.o Boot/*.o Drivers/*.o Kernel/*.oasm Boot/*.oasm Drivers/*.oasm
# run: KelpOs.iso
# 	$(EMU) $(EMU_FLAGS) $<

# Kernel.elf: $(OBJS)
# 	$(LD) $(LD_FLAGS) $(OBJS) -o $@ 

# KelpOs.iso:	Kernel.elf
# 	- rm -rf ./IsoGen
# 	mkdir -p ./IsoGen/boot/grub
# 	cp ./Boot/grub.cfg ./IsoGen/boot/grub/grub.cfg
# 	cp Kernel.elf ./IsoGen/boot/Kernel.elf
# 	grub-mkrescue -o $@ ./IsoGen

# %.o: %.c
# 	$(CC) $(CC_FLAGS) $< -o $@

# %.oasm: %.asm
# 	$(AS) $(AS_FLAGS) $< -o $@