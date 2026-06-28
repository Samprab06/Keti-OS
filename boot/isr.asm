global isr_default

global keyboard_isr
extern keyboard_handler

global timer_isr
extern timer_handler

global fault_handle
extern page_fault_handler

global syscall_isr
extern syscall_handler

global mouse_isr
extern mouse_handler

section .text

keyboard_isr:
    pusha
    call keyboard_handler
    popa
    iret 

mouse_isr:
    pusha
    call mouse_handler
    popa
    iret

fault_handle:
    pusha
    push dword [esp+32]    ; push error code (sits above pusha's 8 regs)
    call page_fault_handler
    add esp, 4
    popa
    add esp, 4             ; remove error code CPU pushed
    iret

isr_default:
    iret

timer_isr:
    pusha
    call timer_handler
    popa
    iret

syscall_isr:
    pusha
    push ecx
    push ebx
    push eax
    call syscall_handler
    add esp, 12
    popa
    iret

section .note.GNU-stack noalloc noexec nowrite progbits