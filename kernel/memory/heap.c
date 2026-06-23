#include "heap.h"
#define HEAP_START 0x400000
#define HEAP_SIZE  0x100000

struct kmalloc_entry *heap_head;

void heap_init(){
    heap_head = (struct kmalloc_entry *)HEAP_START;
    heap_head-> size = HEAP_SIZE - sizeof(struct kmalloc_entry);
    //0 be not free (or used) and 1 be free
    heap_head-> flag = 1;
    heap_head-> next = 0;
}

void *kmalloc(unsigned int req){
    struct kmalloc_entry *current = heap_head;

    while (current!=0){
        if (current->flag == 1 && current->size >= req){
            
            unsigned int leftover = current->size - req - sizeof(struct kmalloc_entry);
            struct kmalloc_entry *new_block = (struct kmalloc_entry *)((unsigned char *)(current + 1) + req);

            new_block->size = leftover;
            new_block->flag = 1;
            new_block->next = current->next;

            current->size = req;
            current->flag = 0;
            current->next = new_block; 
            return (void *)(current+1);

        }
        current = current->next;
    }
    return 0;
}

void kfree(void *ptr){
    struct kmalloc_entry *block = (struct kmalloc_entry *)ptr - 1;
    block->flag = 1;
}