#include "mouse.h"
#include "drivers/ports.h" 
#include "cpu/idt.h"
#include "display/vga.h"  

extern void mouse_isr();
static int count=0;
static unsigned char inp_byte[3];
int pointer_x;
int pointer_y;

void mouse_wait_read(){
    while(!(inb(0x64) & 0x1));
}

void mouse_handler(){
    unsigned char data = inb(0x60);
    inp_byte[count] = data;
    count++;
    if (count == 3) {
        count = 0;
        //first byte deals with the y overflow, x overflow, y sign, x sign, button (middle, right and left) 
        unsigned char b1 = inp_byte[0];

        //second byte is the X movement
	    unsigned char b2 = inp_byte[1];
        //
	    pointer_x = (int)(b2 - ((b1 << 4) & 0x100));
        //third byte is the y movement
	    unsigned char b3 = inp_byte[2];
	    pointer_y = (int)(b3 - ((b1 << 3) & 0x100));
    } 
    outb(0x20, 0x20);
    outb(0xA0, 0x20);
}

void mouse_init(){
    outb(0x64,0xA8);
    
    outb(0x64, 0x20);
    
    mouse_wait_read();//wait before every read cuz you wanna make sure 
    // //a delay to make sure its registered
    unsigned char inp = inb(0x60);
    
    inp|=0x02;//set bit 1 to 1 00000010 | inp => bit one =1. we do this to enable the aux input at IRQ12
    outb(0x64, 0x60); //set the to value back into the port

    outb(0x60, inp);//set the value from the previous input back into the data port
    
    outb(0x64, 0xD4);
    outb(0x60, 0xF4);

    mouse_wait_read();
    inb(0x60);
    outb(0xA1, 0xEF);
    idt_fill(44, (unsigned int) mouse_isr);
}