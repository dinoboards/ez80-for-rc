
# Firmware support for CBIOS

The `fbios-xxx.x` files provide disk bios services for CP/M and other similar Operating Systems.

It exposes a set of jump points at address $000180 (see fbios-jmp-table-adl.s)


## Booting off storage process

| Step | Label                            | Description                                                                                           |
| ---- | -------------------------------- | ----------------------------------------------------------------------------------------------------- |
| 1    | _rst0                            | Internal ROM firmware starts at address $000000 (see vectors16.asm:_rst0)                             |
| 2    | __init                           | Ports and eZ80 are configured (see init-f92.asm:__init)                                               |
| 3    | _main                            | Further eZ80 configuration is performed (see main.s:_main)                                            |
| 4    | boot_prompt1                     | additional boot report                                                                                |
| 5    | _main                            | Probing of linear RAM and external 16 bit ROMS                                                        |
| 6    | _main                            | if no linear RAM, boot external or report NO OS and halt                                              |
| 7    | boot_storage                     | start of code to boot from storage system (eg usb flash drive)                                        |
| 8    | boot_prompt2                     | additional boot report                                                                                |
| 9    | allocate_ram_for_hbios_and_fbios | allocate 64K segment for HBIOS and CBIOS variables (see probe-mem.c:allocate_ram_for_hbios_and_fbios) |
| 10   | _hbios_cio_init                  | scan, 'install' & report hbios level disk drivers (eg: RAM and USG SCSI) into `hb_storage_units`      |
| 11   | _hbios_dio_init                  | scan, 'install' & report hbios level disk drivers (eg: RAM and USG SCSI) into `hb_storage_units`      |
| 12   | _os_boot                         | Allocate a 64K segment to load OS into (see fbios-boot.s:_cpm_boot)                                   |
| 13   | FB_INIT                          | Remount all fbios disks & identify boot disk image                                                    |
| 14   | fbios_mount_disks                | FBIOS iterates the HBIOS storage devices and loads `fb_storage_units` (see fbios-mount-disks.c)       |
| 15   | fbios_find_boot_disk             | Find FBIOS disk number containing bootable operating system                                           |
| 16   | FB_RELOAD                        | Reload operating system sectors from initial boot disk sectors as per boot sector params              |
| 17   | _BOOT                            | jump to CCP (CCP_LOC)                                                                                 |


> At the moment the OS's CBIOS is embedded in the ROM and not loaded from disk.

## TODO

1. ~~rename firmware's CBIOS to FBIOS (firmware CBIOS helper)~~
2. ~~Create a relocatable loader routine (stored in first 3 sectors of boot disk) to load OS and its CBIOS~~ changed approach
3. ~~Refactor the probing and startup process~~
4. ~~update cbios to create the DBP and DPH for each mounted disk~~
5. ~~implement out of memory check for cbios heap allocation~~
6. ~~confirm multiple slices are mounted~~
7. ~~implement disk writing~~
8. implement usb floppy
9. ~~implement a RAM Disk~~
10. ~~implement basic disk read cache (512 to 128 sector matching)~~
11. update hbios/fbios drive mapping - add as many slices as possible - eg: dont limit to 4 is only one disk partition is found
12. ~~ensure boot detect internal/external mode correctly~~
13. ~~update/correct firmware boot reporting~~
14. print_string/printf assumes UART0 output - will not be correct when CRT is enabled
15. implement hbios cio directly - bypass rst-10 jumps
16. update hbios device reporting to a central reporting pattern
17. implement CRT output using V9958/HDMI
18. implement USB Keyboard to pair with CRT
19. Implement matrix keyboard
20. Implement Compact Flash storage
21. refactor locations of function to correct namespace/scope (fbios-get-lsb.c and others)
22. refactor c code to optimise code size
23. ~~add eZ80.COM to disk image~~
24. ~~add an xmodem util~~
25. add WDATE.COM - with appropriate local HBIOS support
26. add TIMER.COM - with appropriate local HBIOS support



## Boot Order Logic

1. If external ROM detected, boot from that
2. If linear RAM and storage with boot loader, boot from storage
3. Otherwise, display "NO OS" and halt.

