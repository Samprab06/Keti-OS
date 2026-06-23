#include "display/vga.h"
#include "cpu/gdt.h"
#include "cpu/idt.h"
#include "drivers/timer.h"
#include "drivers/keyboard.h"
#include "memory/multiboot.h"
#include "memory/paging.h"
#include "memory/heap.h"

void kernel_main(unsigned int multiboot_ptr) {
    gdt_init();
    idt_init();
    keyboard_init();
    clear_vga();
    print_vga("Keti OS\nBy Aadithya Vasudevan\nVersion 1.0");
    timer_init();
    innit((struct multiboot_info *)multiboot_ptr);
    paging_init();
    heap_init();


    // print_hex_vga(alloc_frame());
    // print_char_vga('\n');
    // print_hex_vga(alloc_frame());

    //testing fault handling
    // unsigned int *bad = (unsigned int *)0x2000000;   // 32MB - unmapped
    // *bad = 42; 

    // void *a = kmalloc(100);
    // print_hex_vga((unsigned int)a);
    // print_char_vga('\n');

    // void *b = kmalloc(200);
    // print_hex_vga((unsigned int)b);
    // print_char_vga('\n');

    // kfree(a);

    // void *c = kmalloc(50);
    // print_hex_vga((unsigned int)c);
    // print_char_vga('\n');
    
    __asm__ volatile ("sti");
    while(1) {}
}