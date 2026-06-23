#ifndef IDT_H
#define IDT_H


struct idt_entry {
    unsigned short low_handler_addr;
    unsigned short seg_select;
    unsigned char zero;
    unsigned char types;
    unsigned short high_handler_addr;
} __attribute__((packed));

struct idt_descriptor {
    unsigned short size;
    unsigned int address;
} __attribute__((packed));

void idt_init();
void idt_fill(int i, unsigned int handler);

#endif