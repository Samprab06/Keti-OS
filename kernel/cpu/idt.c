#include "idt.h"
#include "drivers/ports.h"

void pic_remap();
void idt_load();
void isr_default();

//idt allows 0-255
struct idt_entry idt[256];
struct idt_descriptor idt_desc;

#define PIC1_PORT_A 0x20
#define PIC2_PORT_A 0xA0

extern void isr_default();
extern void fault_handle();

void idt_fill(int i, unsigned int handler){
    idt[i].low_handler_addr = handler & 0xFFFF; //lower 16
    idt[i].seg_select = 0x08;
    idt[i].zero = 0;
    idt[i].types = 0x8E;
    idt[i].high_handler_addr = (handler>>16) & 0xFFFF;;
}

void idt_init(){
    
    for (int i = 0; i < 256; i++) {
        idt_fill(i, (unsigned int)isr_default);
        if (i == 14){
        }
    }
    idt_fill(14, (unsigned int)fault_handle);

    pic_remap();
    idt_desc.size = (sizeof(struct idt_entry) * 256) - 1; //
    idt_desc.address = (unsigned int)&idt;
    idt_load();
}

void pic_remap() {
    outb(0x20, 0x11);
    outb(0xA0, 0x11);

    outb(0x21, 0x20);
    outb(0xA1, 0x28);

    outb(0x21, 0x04);
    outb(0xA1, 0x02);

    outb(0x21, 0x01);
    outb(0xA1, 0x01);

    outb(0x21, 0xFD); //very important to show keypress, when firing everything except keyboard gets masked
    outb(0xA1, 0xFF); //all slave IRQ also gets masked
}