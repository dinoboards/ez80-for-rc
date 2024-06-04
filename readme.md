
## RC2014 eZ80 Firmware

This repo contains the code required for operating the eZ80 CPU on the RC2014 platform.

The firmware will manage startup, configuration of on-chip devices, interrupt marshalling and handover to external ROM/RAM modules in Z80 compatibility mode.

## Building the code

This is a ZDSII ez80Acclaim (5.3.5) project.

It requires windows to be built and be placed in a specific absolute path under windows:

Recommend to:
* Clone repo to `Z:\rc2014-ez80-firmware`
* Install the ZDS IDE to `Z:\ZDS`

> Many of the configuration files created and managed by the ZDSII IDE include full absolute paths to files.  So if you try open the project file
under a different local path, many of the project files will be updated.

The Zilog ZDS IDE can be found at: https://zilog.com/index.php?option=com_zcm&task=view&soft_id=54&Itemid=74

This is a very old tool set.  But it does allow for on-chip debugging.  It be nice though to get a more modern tool chain to manage this code base.
