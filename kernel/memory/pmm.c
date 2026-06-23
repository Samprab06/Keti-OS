#include "multiboot.h"

//physical mapping of memory

//im going to carry the bitmapping method for now, as it grows, I want to learn the linked list method and then implement it

//bitmapping is basically /8 and %8 spam if you think about it

unsigned char bitmap[131072]; //4GB ram, so 4*1024*1024*1024 / (4096 * 8) = 131072 bytes (so converted the GB to Bytes, then to Frames, then to Bytes of bitmap)

extern unsigned int kernel_end;
unsigned int end_addr = (unsigned int)&kernel_end;

void bitmap_set(int frame){
    bitmap[frame/8] |= (1<<(frame%8));
}

void bitmap_clear(int frame) {
    bitmap[frame / 8] &= ~(1<<(frame%8));
}

int bitmap_test(int frame) {
    return bitmap[frame / 8] & (1 << (frame % 8));
}

//may be confusing at first cuz we want to see if it is used or not
//so this functions checks if a frame is free and sets it to used
unsigned int alloc_frame(){
    for (int i = 0; i < 1048576; i++){
        if (bitmap_test(i) == 0){ //if frame free
            bitmap_set(i); //set it to used
            return i*4096; //address
        }
    }
    return 0; //if nothing found or out of bounds
}

void free_frame(unsigned int addr){
    int frame = addr/4096;
    bitmap_clear(frame);
}

void innit(struct multiboot_info *mb_info){ //not a type mistake innit
    //we can break this into 3 steps

    //1. sets every frame to used
    for (int i = 0; i < 1048576; i++){
        bitmap_set(i); //set it to used
    }

    //2. free every frame from the entry addr to the end itself
    unsigned int current = mb_info->mmap_addr;
    while (current < mb_info->mmap_addr + mb_info->mmap_length){
        struct multiboot_mmap_entry *entry = (struct multiboot_mmap_entry *)current;

        if (entry->type==1){
            unsigned int addr = entry->addr;
            unsigned int end = entry->addr + entry->len;
            while (addr < end) {
                free_frame(addr);
                addr += 4096;
            }
        }
        current += entry->size + 4;
    }

    //3. make sure kernel memory is never handed out
    unsigned int addr = 0;
    while (addr < end_addr) {
        bitmap_set(addr/4096);
        addr+=4096;
    }
}  