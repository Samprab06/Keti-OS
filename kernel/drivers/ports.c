#include "ports.h"

void outb(unsigned short port, unsigned char value) {
    __asm__ volatile ("outb %1, %0" : : "dN"(port), "a"(value));
}

unsigned char inb(unsigned short port) {
    unsigned char value;
    __asm__ volatile ("inb %1, %0" : "=a"(value) : "dN"(port));
    return value;
}