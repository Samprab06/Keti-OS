global isr_default
global keyboard_isr
extern keyboard_handler
global timer_isr
extern timer_handler


section .text

keyboard_isr:
    pusha
    call keyboard_handler
    popa
    iret 


isr_default:
    iret

timer_isr:
    pusha
    call timer_handler
    popa
    iret

section .note.GNU-stack noalloc noexec nowrite progbits