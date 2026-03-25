LOADER_START = 0x008000;

ENTRY(_loader)

SECTIONS {
    . = LOADER_START;  /* Set the start address to 0x0000 (or your desired start address) */

    /* Combine your custom .code section into the standard .text output section */
    .text : {
        *(.code)
        *(.text)
        *(.text.*)
    }

    /* It's good practice to handle data/bss even if empty */
    .data : { *(.data) }

    .bss (NOLOAD) : { *(.bss) }
}
