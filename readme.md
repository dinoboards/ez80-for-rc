
## Goals
1. ~~Confirm the EZ80 module works~~
2. Integrate into RC2014 platform and drive some simple leds via I/O regsters
3. Access existing RC2014 16-bit bus ram
4. run the mbrot c code - accessing the VDP directly??
5. Create a simple monitoring kernel - monitors when it is given bus and then execute code as requested

## TODOS

1. Confirm memory map configuration for target
2. How will interrupts be managed
3. ~~Flash code on boot, then flips a PB0 output~~
4. install into main PCB
5. How do we add WAIT STATES FOR External IO
6. CS2 - must be set for IO for range ?? (not 00xx)
7. CS3 - must be set for RAM range? say (80 xxxx)


### 5. How do we add WAIT STATES FOR External IO

Set the top 3 bits of Chip Select Control Register to 7 to add max wait states

Initially I set the Wait states for CS3 and CS2 to 7

### CS0 and CS1 - configuration

| Name      | range | values | notes             |
|-----------|-------|--------|-------------------|
| CSx_EN    | [3]   | 0      | disabled          |

### CS2 - configuration

#### Chip Select Control Register

| Name      | range | values | notes             |
|-----------|-------|--------|-------------------|
| CSx_WAIT  | [7:5] | 111    | 7 wait states     |
| CSX_IO    | [4]   | 1      | IO Select         |
| CSx_EN    | [3]   | 1      | enabled           |

0xF8

#### Bus Mode Control Register

| Name      | range | values | notes             |
|-----------|-------|--------|-------------------|
| BUS_MODE  | [7:6] | 01     | Z80 bus mode      |
| AD_MUX    | [5]   | 0      | Sep Addr/Data     |
| BUS_CYCLE | [3:0] | 1111   | slow bus cycles?? |

0x4F

#### LBR Reset

| Name      | range | values | notes             |
|-----------|-------|--------|-------------------|
| CSx_LBR   | [7:0] | 0xFF   | for IO range FFxx |

### CS3 - configuration

#### Chip Select Control Register

| Name      | range | values | notes             |
|-----------|-------|--------|-------------------|
| CSx_WAIT  | [7:5] | 111    | 7 wait states     |
| CSX_IO    | [4]   | 0      | Memory Select     |
| CSx_EN    | [3]   | 1      | enabled           |

0xE8

#### Bus Mode Control Register

| Name      | range | values | notes             |
|-----------|-------|--------|-------------------|
| BUS_MODE  | [7:6] | 01     | Z80 bus mode      |
| AD_MUX    | [5]   | 0      | Sep Addr/Data     |
| BUS_CYCLE | [3:0] | 1111   | slow bus cycles?? |

0x4F

#### LBR Reset

| Name      | range | values | notes             |
|-----------|-------|--------|-------------------|
| CSx_LBR   | [7:0] | 0x80   | From 0x80         |

#### UBR Reset

| Name      | range | values | notes             |
|-----------|-------|--------|-------------------|
| CSx_UBR   | [7:0] | 0x80   | To 0x80           |


## Address Map

### IO

0000-00FF -> reserved for EZ80 registers

XX00-XXFF -> where XX is non zero is for RC2014 registers

### Memory

Onboard ROM 00 0000 - 01 FFFF
Onboard RAM B7 E000 - B7 FFFF



