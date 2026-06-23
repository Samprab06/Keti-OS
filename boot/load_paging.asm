global load_paging
extern page_directory
section .text

load_paging:
    mov eax, page_directory
    mov cr3, eax ;not cr7 >-<

    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax
    ret

section .note.GNU-stock noalloc noexec nowrite progbits