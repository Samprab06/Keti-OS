global loader
extern kernel_main ; tells NASM this function lives in a C file

MAGIC_NUMBER equ 0x1BADB002
FLAGS        equ 0x0
CHECKSUM     equ -MAGIC_NUMBER

section .text
    align 4
    dd MAGIC_NUMBER
    dd FLAGS
    dd CHECKSUM

loader:
    mov esp, kernel_stack + STACK_SIZE ; set up stack for C
    push ebx
    call kernel_main ; jump into C
.loop:
    jmp .loop ; if C ever returns, hang

STACK_SIZE equ 4096
section .bss
kernel_stack: resb STACK_SIZE ; reserve 4KB for the stack

section .note.GNU-stack noalloc noexec nowrite progbits