#ifndef GDT_H
#define GDT_H

struct gdt_entry {
    unsigned short low_limit;
    unsigned short low_base;
    unsigned char mid_base;
    unsigned char access_byte;
    unsigned char lim_flag_high;
    unsigned char high_base;
} __attribute__((packed));

//If you wanna know more about this stuff, you can check out Little OS book
//they have done a good job explaining the need for packed structs
//it prevents any padding to align and messes with the layout

struct gdt_descriptor {
    unsigned short size;
    unsigned int address;
} __attribute__((packed));

void gdt_init();
void gdt_load();

#endif