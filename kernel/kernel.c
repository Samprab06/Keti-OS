#include "vga.h"
#include "gdt.h"
#include "idt.h"
#include "keyboard.h"

void kernel_main() {
    gdt_init();
    idt_init();
    keyboard_init();
    clear_vga();
    print_vga("Keti OS\nBy Aadithya Vasudevan\nVersion 1.0");
    __asm__ volatile ("sti");
    while(1) {}
}