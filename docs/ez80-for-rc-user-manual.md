---
title: eZ80 for RC User Manual
author: Dean Netherton
copyright: (c) 2025
titlepage-logo: "/docs/dino.png"
logo-width: 60mm
...
---
classoption:
- oneside
titlepage: true
titlepage-rule-color: "fcc603"
titlepage-rule-height: 4
fontsize: 9pt
papersize: A5
numbersections: true
secnumdepth: 2
colorlinks: true
listings-disable-line-numbers: true
geometry:
- top=2.3cm
- bottom=3cm
- left=1.8cm
- right=1.8cm
header-includes:
- |
  ```{=latex}
  \usepackage{awesomebox}
  \usepackage{wrapfig}
  \usepackage{background}
  \renewcommand{\linethickness}{10pt}
  \definecolor{abnote}{RGB}{255, 204, 0}
  \definecolor{yellow}{RGB}{255, 204, 0}
  \definecolor{red}{RGB}{208, 72, 72}
  \definecolor{orange}{RGB}{243, 185, 95}
  \definecolor{blue}{RGB}{104, 149, 210}
  \backgroundsetup{
    position=current page.east,
    angle=-90,
    nodeanchor=east,
    vshift=-3mm,
    opacity=0,
    scale=3,
    contents=Draft
  }
  ```
pandoc-latex-environment:
  noteblock: [note]
  tipblock: [tip]
  warningblock: [warning]
  cautionblock: [caution]
  importantblock: [important]
hyperrefoptions:
  draft
...


---
numbersections: false
...

\newpage
\hypersetup{linkcolor=}
\tableofcontents
\newpage


# eZ80 for RC - User Manual

This document provides a high level overview of the eZ80 for RCBus system and how to operate it.

It assumes you have successfully assembled and booted up a working system.  It also assume you have at least a basic understanding of CP/M

## Minimum System Configurations

A minimum configuration to enable a booting system, requires:
1. the `eZ80 for RC` module
2. a 512K banked RAM/ROM module (with the eZ80 configured build of RomWBW flashed to the ROM).
3. An RC2014 or RCBus compatible backplane
4. A serial terminal connected to the `eZ80 for RC` serial connector

### Optional Extras in the Green Machine Series

1. `USB for RC` enabling external flash & HDD storage, Floppy Disks, Keyboard and any other supported devices.
2. `2MB Linear SRAM` module to give your eZ80 up to 2 Megabytes of continuous memory.


## Memory Layout

The eZ80 can address up to 16MB of continuous memory and address up to 64KB of I/O ports. And supports 4 Chip Select signals that can be independently configured for specific address ranges and specific access timings.

The following table details how the memory has been configured for the system:

|   Address Range   | Description |
|-------------------|-------------|
| 000000 -> 01FFFF	| ON-CHIP ROM |
| 02E000 -> 02FFFF	| ON-CHIP RAM  |
| 030000 -> 03FFFF	| EXTERNAL BANKED ROM/RAM (CS3) |
| 200000 -> 3FFFFF	| EXTERNAL FLAT RAM (CS0) 2MB |
| 400000 -> FFFFFF	| RESERVED |

CS2 is wired for external I/O access onto appropriate RC2014/RC Bus signals.  The following  table details how the I/O addressing is mapped:"

| Address Range | Description |
|-------------------|-------------|
| 0000 -> 00FF	|	Reserved ON-CHIP IO |
| FF00 -> FFFF	|	External IO (CS2) |


## Bootup process

On power up, the onboard 'firmware' of the eZ80, stored in its on-chip ROM, starts.  This firmware code will initialise the CPU and on-chip services, including configuring the on-chip UART.
After this initialising of the on-chip hardware, it will delegate control the external bank ROM/RAM module - typyicaly this will contain a copy of RomWBW software allowing for the system to boot into an operating system such as CP/M.

The firmware's startup initialisation will perform the following tasks:

1. Identify if the on-chip Real-Time-Clock (RTC) is powered.
2. If RTC is available, use it to measure and determine the CPU's actual clock frequency (eg: 20Mhz or 25Mhz)
3. Configure the on-chip UART serial baud rate to 115200. *
4. Configure the Chip Select lines (CS0 to CS3) to appropriate wait states for reliable access to memory and I/O devices.


