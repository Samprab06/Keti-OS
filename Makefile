ASM = nasm
CC = i686-linux-gnu-gcc
CFLAGS = -m32 -ffreestanding -Ikernel
LD = ld
BUILD_DIR = build

all: run

# Boot assembly
boot/loader.o: boot/loader.asm
	$(ASM) -f elf32 boot/loader.asm -o boot/loader.o

boot/gdt_load.o: boot/gdt_load.asm
	$(ASM) -f elf32 boot/gdt_load.asm -o boot/gdt_load.o

boot/idt_load.o: boot/idt_load.asm
	$(ASM) -f elf32 boot/idt_load.asm -o boot/idt_load.o

boot/isr.o: boot/isr.asm
	$(ASM) -f elf32 boot/isr.asm -o boot/isr.o

boot/load_paging.o: boot/load_paging.asm
	$(ASM) -f elf32 boot/load_paging.asm -o boot/load_paging.o

# Kernel core
kernel/kernel.o: kernel/kernel.c
	$(CC) $(CFLAGS) -c kernel/kernel.c -o kernel/kernel.o

# Display
kernel/display/vga.o: kernel/display/vga.c
	$(CC) $(CFLAGS) -c kernel/display/vga.c -o kernel/display/vga.o

# CPU
kernel/cpu/gdt.o: kernel/cpu/gdt.c
	$(CC) $(CFLAGS) -c kernel/cpu/gdt.c -o kernel/cpu/gdt.o

kernel/cpu/idt.o: kernel/cpu/idt.c
	$(CC) $(CFLAGS) -c kernel/cpu/idt.c -o kernel/cpu/idt.o

# Drivers
kernel/drivers/ports.o: kernel/drivers/ports.c
	$(CC) $(CFLAGS) -c kernel/drivers/ports.c -o kernel/drivers/ports.o

kernel/drivers/keyboard.o: kernel/drivers/keyboard.c
	$(CC) $(CFLAGS) -c kernel/drivers/keyboard.c -o kernel/drivers/keyboard.o

kernel/drivers/timer.o: kernel/drivers/timer.c
	$(CC) $(CFLAGS) -c kernel/drivers/timer.c -o kernel/drivers/timer.o

# Memory
kernel/memory/pmm.o: kernel/memory/pmm.c
	$(CC) $(CFLAGS) -c kernel/memory/pmm.c -o kernel/memory/pmm.o

kernel/memory/paging.o: kernel/memory/paging.c
	$(CC) $(CFLAGS) -c kernel/memory/paging.c -o kernel/memory/paging.o

kernel/memory/heap.o: kernel/memory/heap.c
	$(CC) $(CFLAGS) -c kernel/memory/heap.c -o kernel/memory/heap.o

# Link
OBJS = boot/loader.o boot/gdt_load.o boot/idt_load.o boot/isr.o boot/load_paging.o \
       kernel/kernel.o kernel/display/vga.o kernel/cpu/gdt.o kernel/cpu/idt.o \
       kernel/drivers/ports.o kernel/drivers/keyboard.o kernel/drivers/timer.o \
       kernel/memory/pmm.o kernel/memory/paging.o kernel/memory/heap.o

$(BUILD_DIR)/kernel.elf: $(OBJS)
	mkdir -p $(BUILD_DIR)
	$(LD) -T link.ld -melf_i386 $(OBJS) -o $(BUILD_DIR)/kernel.elf

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
	rm -f $(OBJS)

.PHONY: all run clean