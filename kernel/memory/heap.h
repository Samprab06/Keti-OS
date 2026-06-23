#ifndef HEAP_H
#define HEAP_H

void heap_init();
void *kmalloc(unsigned int req);
void kfree(void *ptr);

struct kmalloc_entry {
    unsigned int size;
    unsigned char flag; //checks if the frame is free or used
    struct kmalloc_entry *next;
} __attribute__((packed));

#endif