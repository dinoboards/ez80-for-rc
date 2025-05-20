## EXE loader

This CP/M utility will load relocatable `.EXE` applications into extended memory.

`.EXE` files are like standard CP/M `.COM` files, but are executed in the eZ80 ADL addressing mode, allowing for applications larger then 64K.

By default, it will search appropriate installed extended memory and relocate the application into it.


### Usage

<code>
  Usage: EXE [options] FILENAME [arguments]

  Load and execute an eZ80 executable program. The file must have
  the .EXE extension. Any additional arguments after FILENAME
  will be passed to the executed program.

  Options:
    -F          Use fastest available RAM bank (default)
    -M          Use maximum continuous RAM space
    -T=HH       Load into specific target address where HH
                is a hex value between 20-58 (e.g. -T=40)
    -? /?       Show this help message

  Examples:
    EXE PROGRAM         Run in fastest RAM bank
    EXE -M PROGRAM      Run in largest available RAM
    EXE -T=40 PROGRAM   Run at address 0x400000
    EXE PROGRAM ARG1    Run with argument ARG1
</code>
