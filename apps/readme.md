# CP/M applications for the eZ80 CPU module

These applications are designed specifically for operating on the eZ80 CPU running CP/M under RomWBW's HBIOS.

The applications are compiled/assembled with either `z88dk` or `ez80-clang`

See below for building the applications yourself.

### CUBE

Draws a rotating 3d cube outline.  Requires a V99x8 VDP Module.

### DHRY

A benchmarking tool using the dhrystone measurement process.  See its [readme](./dhry/readme.md).

### EZ80

A tool to report on the eZ80 current bus timing and configuration.

### FWMAIN

A tool to erase the *ALT* firmware and revert back to the *main* default firmware.

### FWUPDATE

A tool to flash an *ALT* firmware onto the eZ80.

### HWADL

A simple hello world example, built using the `ez80-clang` tool chain.  Produced code is eZ80 ADL code.

### HWZ80

A simple hello world example, built using the `z88dk` tool chain.  Produced code is standard Z80 code.

### I2CSAN

A tool to use the i2c interface of the eZ80 (via the firmware) to scan for any connected i2c devices.

### LINE

Draws random colour lines on the VDP.  Requires a V99x8 VDP Module.

### MBROT

Draw a mbrot image.   Requires a V99x8 VDP Module.

### WHET

A benchmarking tool using the whetstone measurement process.  See its [readme](./whet/readme.md).


## Building

### Prerequisites

* Linux - tested with ubuntu/Debian. For windows users, WSL will work
* Docker
* ez80-clang
* gnu make (`apt-get install build-essential`)

#### Installing `ez80-clang`

eZ80-clang is a port of clang to target and build for the eZ80 CPU.  More details at: https://github.com/dinoboards/ez80-clang

To install:

```
wget https://github.com/dinoboards/ez80-clang/releases/download/v0.0.13-rc.51/ez80-clang-v0.0.13-rc.51.tar.gz
tar -xzvf ez80-clang-v0.0.13-rc.51.tar.gz
cd ez80-clang-v0.0.13-rc.51
sudo ./install.sh
```

> Change version as per latest/required release

### Building

within the apps directory of this repo, simply run the `make` command

```
cd apps
make
```
