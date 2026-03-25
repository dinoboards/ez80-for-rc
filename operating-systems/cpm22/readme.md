

## Notes on choice of tool chain for CBIOS

### WINE/ZDS
At the moment, the cbios is compiled under wine/zds - requiring wine and ZDS to be installed.

ZDS is not really best suited for such targeted build type

Advantage:
* same tool chain as main firmware
* keeps syntax similar

Disadvantage:
* non-trival setup of tool chain
* not suited for producing stand alone bins
* segment/space mapping is a bit strange
* slowish
* requires wine wrapper
* integrating c runtime might be challenging


### ez-as/ez-ld

An alternative approach to compiling/assembling the cbios is to use the ez80-clang/assembler tool chain - this would also need to be installed

Advantage
* modern assembler/compiler
* simple tool chain install for linux

Disadvantage:
* syntax of assembly is different
* integrating c runtime might be challenging
*

Example of assembling:
```
ez80-as -march=ez80+full cbios-z80.s -o cbios-z80.o
ez80-as -march=ez80+full spike.s -o spike.o
```

Example of linking:
```
ez80-ld -T cbios.ld cbios-z80.o spike.o --oformat binary -o "spike.bin" --Map "spike.map"
```

The associated cbios.ld file:

```
CBIOS_START = 0x00E600;

ENTRY(_cbios)

SECTIONS {
    . = CBIOS_START;  /* Set the start address to 0x0000 (or your desired start address) */

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
```
