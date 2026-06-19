global idt_load
extern idt_desc

section .text

idt_load:
    lidt [idt_desc]
    ret

section .note.GNU-stack noalloc noexec nowrite progbits