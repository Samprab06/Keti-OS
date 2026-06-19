global isr_default
global keyboard_isr
extern keyboard_handler

section .text

keyboard_isr:
    pusha
    call keyboard_handler
    popa
    iret 


isr_default:
    iret

section .note.GNU-stack noalloc noexec nowrite progbits