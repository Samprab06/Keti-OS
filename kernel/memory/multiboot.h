#ifndef MULTIBOOT_H
#define MULTIBOOT_H

struct multiboot_info {
    unsigned int flags; //tells the validity of the field, flag helps us tell if the relevant bit is set
    unsigned int mem_lower; //RAM existing below 1MB
    unsigned int mem_upper; //above 1MB
    unsigned int boot_device; //tells disk and partition 
    unsigned int cmdline; //address of command line args
    unsigned int mods_count; //modules to be loaded
    unsigned int mods_addr; //address of said modules
    unsigned int syms[4]; //debug mainly
    unsigned int mmap_length;    // total size of the memory map
    unsigned int mmap_addr;      // address where the memory map starts
} __attribute__((packed));

struct multiboot_mmap_entry {
    unsigned int size;
    unsigned long long addr;
    unsigned long long len;
    unsigned int type;
} __attribute__((packed));

void innit(struct multiboot_info *mb_info);

void free_frame(unsigned int addr);

unsigned int alloc_frame();

void bitmap_set(int frame);

void bitmap_clear(int frame);

int bitmap_test(int frame);

#endif