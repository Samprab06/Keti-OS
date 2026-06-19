#include "gdt.h"

struct gdt_entry gdt[3];
struct gdt_descriptor gdt_desc;

void gdt_set_entry(int i, unsigned int base, unsigned int limit, unsigned char access, unsigned char flags){
        gdt[i].low_limit = limit & 0xFFFF; //lower 16
        gdt[i].low_base = base & 0xFFFF; //lower 16
        gdt[i].mid_base = (base >> 16) & 0xFF; //mid 8
        gdt[i].access_byte = access; 
        gdt[i].lim_flag_high = ((limit >> 16) & 0x0F) | (flags << 4); //upper 4 only and upper 4 of flag
        gdt[i].high_base = (base >> 24) & 0xFF; //upper 8
}

void gdt_init(){
    gdt_set_entry(0, 0, 0, 0, 0);
    gdt_set_entry(1, 0x0, 0xFFFFF, 0x9A, 0xC); //cs
    gdt_set_entry(2, 0x0, 0xFFFFF, 0x92, 0xC); //ds
    gdt_desc.size = (sizeof(struct gdt_entry) * 3) - 1;
    gdt_desc.address = (unsigned int)&gdt;
    gdt_load();
}

