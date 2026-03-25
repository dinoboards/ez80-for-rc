CBIOS_START = 0x00E600;
TOP_PAGE = 0x00FF00;

ENTRY(_cbios)

SECTIONS {
    . = CBIOS_START;

    .text : {
        *(.code)
        *(.text)
        *(.text.*)
    }

    .data : { *(.data) }

    .bss (NOLOAD) : { *(.bss) }

    HEAP = .;

    . = TOP_PAGE;

    HEAP_END = TOP_PAGE;
    HEAP_SIZE = HEAP_END-HEAP;
}
