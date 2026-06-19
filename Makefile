ASM = nasm
CC = i686-linux-gnu-gcc
LD = ld
BUILD_DIR = build

all: run

# Assemble the boot loader
boot/loader.o: boot/loader.asm
	$(ASM) -f elf32 boot/loader.asm -o boot/loader.o

boot/gdt_load.o: boot/gdt_load.asm
	$(ASM) -f elf32 boot/gdt_load.asm -o boot/gdt_load.o

boot/idt_load.o: boot/idt_load.asm
	$(ASM) -f elf32 boot/idt_load.asm -o boot/idt_load.o

boot/isr.o: boot/isr.asm
	$(ASM) -f elf32 boot/isr.asm -o boot/isr.o

# Compile the C kernel
kernel/kernel.o: kernel/kernel.c
	$(CC) -m32 -ffreestanding -c kernel/kernel.c -o kernel/kernel.o

kernel/vga.o: kernel/vga.c
	$(CC) -m32 -ffreestanding -c kernel/vga.c -o kernel/vga.o
	
kernel/gdt.o: kernel/gdt.c
	$(CC) -m32 -ffreestanding -c kernel/gdt.c -o kernel/gdt.o

kernel/idt.o: kernel/idt.c
	$(CC) -m32 -ffreestanding -c kernel/idt.c -o kernel/idt.o

kernel/keyboard.o: kernel/keyboard.c
	$(CC) -m32 -ffreestanding -c kernel/keyboard.c -o kernel/keyboard.o

kernel/ports.o: kernel/ports.c
	$(CC) -m32 -ffreestanding -c kernel/ports.c -o kernel/ports.o

# Link everything into a kernel ELF
$(BUILD_DIR)/kernel.elf: boot/loader.o boot/gdt_load.o boot/idt_load.o boot/isr.o kernel/kernel.o kernel/vga.o kernel/gdt.o kernel/idt.o kernel/keyboard.o kernel/ports.o
	mkdir -p $(BUILD_DIR)
	$(LD) -T link.ld -melf_i386 boot/loader.o boot/gdt_load.o boot/idt_load.o boot/isr.o kernel/kernel.o kernel/vga.o kernel/gdt.o kernel/idt.o kernel/keyboard.o kernel/ports.o -o $(BUILD_DIR)/kernel.elf

# Build the bootable ISO
$(BUILD_DIR)/keti.iso: $(BUILD_DIR)/kernel.elf
	mkdir -p $(BUILD_DIR)/isodir/boot/grub
	cp $(BUILD_DIR)/kernel.elf $(BUILD_DIR)/isodir/boot/kernel.elf
	cp boot/grub.cfg $(BUILD_DIR)/isodir/boot/grub/grub.cfg
	grub-mkrescue -o $(BUILD_DIR)/keti.iso $(BUILD_DIR)/isodir



# Boot in QEMU
run: $(BUILD_DIR)/keti.iso
	qemu-system-i386 -cdrom $(BUILD_DIR)/keti.iso -boot d

clean:
	rm -rf $(BUILD_DIR)
	rm -f boot/loader.o kernel/kernel.o kernel/vga.o boot/gdt_load.o boot/isr.o boot/idt_load.o kernel/gdt.o kernel/idt.o kernel/keyboard.o kernel/ports.o

.PHONY: all run clean