MEMORY
{
    RAM (rwx) : ORIGIN = 0x0100, LENGTH = 64K-0x100
}

SECTIONS
{
    /* . = 0x0100; */

    CODE : {
        *(.text)
    } > RAM

    DATA : {
        *(.data)
    } > RAM

    BSS (NOLOAD) : {
        *(.bss)
    } > RAM
}
