ENTRY(loader)

SECTIONS {
    . = 0x00100000;          /* the code should be loaded at 1 MB at the current memory address*/

    .text ALIGN (0x1000) :   /* align at 4 KB */
    {
        *(.text)             /* all text (i.e. code sections) sections from all files */
    }

    .rodata ALIGN (0x1000) : /* align at 4 KB */
    {
        *(.rodata*)          /* all read-only data sections from all files */
        /*any variable that is directly assigned (not mov stuff) is kept here and stuff with *x*/
    }

    .data ALIGN (0x1000) :   /* align at 4 KB */
    {
        *(.data)             /* all data sections from all files */
    }

    .bss ALIGN (0x1000) :    /* align at 4 KB */
    {
        *(COMMON)            /* all COMMON sections from all files */
        *(.bss)              /* all bss sections from all files */
    }
    kernel_end = .;
}