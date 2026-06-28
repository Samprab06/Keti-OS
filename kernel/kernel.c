#include "display/vga.h"
#include "cpu/gdt.h"
#include "cpu/idt.h"
#include "drivers/timer.h"
#include "drivers/keyboard.h"
#include "memory/multiboot.h"
#include "memory/paging.h"
#include "memory/heap.h"
#include "shell/shell.h"
#include "process/pcb.h"
#include "process/scheduler.h"
#include "drivers/mouse.h"

extern void enter_usermode(unsigned int eip);

// void user_task() {
// }

void kernel_main(unsigned int multiboot_ptr) {
    gdt_init();
    idt_init();
    pcb_init();
    keyboard_init();
    
    clear_vga();
    print_vga("Keti OS\nBy Aadithya Vasudevan v0.1\ntype 'help' to get started\n\n");
    timer_init();
    innit((struct multiboot_info *)multiboot_ptr);
    paging_init();
    heap_init();
    //mouse_init();
    shell_init();
    __asm__ volatile ("sti");
    while(1) {}
}
