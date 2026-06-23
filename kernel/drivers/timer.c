#include "timer.h"
#include "ports.h"
#include "cpu/idt.h"

extern void timer_isr();

unsigned int tick = 0;

//1193182Hz at a 100Hz will be 11931
void timer_init(){
    unsigned int top = 11931;
    outb(0x43, 0x36);
    outb(0x40, top & 0xFF); // low byte
    outb(0x40, (top >> 8) & 0xFF); // high byte
    idt_fill(32, (unsigned int)timer_isr); // register handler
}

void timer_handler(){
    tick++;
    timer_isr();
}