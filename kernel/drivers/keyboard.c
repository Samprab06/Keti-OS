#include "keyboard.h"
#include "display/vga.h"
#include "cpu/idt.h"
#include "ports.h"

extern void keyboard_isr();

static char scancode_table[128] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
    '-', '=', 0, 0,
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0,
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\',
    'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, 0, 0, ' '
};

void keyboard_handler(){
    unsigned char scancode = inb(0x60);   
    if (scancode == 0x0E) {
        delete_char_vga();
    } 
    else if(scancode < 128){
        char c = scancode_table[scancode];
        if (c != 0){
            print_char_vga(c);
        }
    }

    outb(0x20, 0x20);
}

void keyboard_init(){
    //register vector 33
    idt_fill(33, (unsigned int)keyboard_isr);
}