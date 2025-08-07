## EZ80.COM

Application for reporting, testing and adjust the timings of memory modules. It is a CP/M application designed to run on an eZ80 CPU.

---

Running **EZ80.COM** with no arguments will report timings:

```text
C>EZ80
          CPU frequency: 25000000 Hz
        Z80 Environment: eZ80's Z80 Compatibility
      On-chip Flash W/S: 1  (80ns)
        Main Memory B/C: 1  (120ns)
Extended Memory CS0 W/S: 2  (120ns)
Extended Memory CS1 W/S: 1  (80ns)
                I/O B/C: 4  (480ns)
         TMR1 Tick rate: 50 (Counter: 11673)
```

To get a list of command line options, run with the `-?` argument

```text
C>EZ80 -?
Usage: ez80 [options]
Options:
  -F=<number>W      Set On-chip Flash Wait States
  -M=<number>[W|B]  Set Main Memory Wait States or Bus Cycles (CS3)
  -M0=<number>[W|B] Set Extended Memory Wait States or Bus Cycles (CS0)
  -M1=<number>[W|B] Set Extended Memory Wait States or Bus Cycles (CS1)
                    Wait States: 0-7, Bus Cycles: 1-15
  -I=<number>[W|B]  Set I/O Wait States or Bus Cycles (CS2)
  -S                Scan extended memory
  -P                Measure Main Memory Performance
  -T=<number>       Set tick frequency rate (50 or 60)
  -? /?             Show this help message
```

As shown in the help above, to scan and verify your extended memory is working run with the `-S` option:

```text
C>EZ80 -S
Searching for memory installed from 0x200000 to 0x5F0000
512K bank at 0x200000: OK
512K bank at 0x280000: OK
512K bank at 0x300000: OK
512K bank at 0x380000: OK
512K bank at 0x400000: None
512K bank at 0x480000: None
512K bank at 0x500000: None
512K bank at 0x580000: None

2048kb available at 0x200000

Memory @ 0x200000: 3471.18 KB/s
```

To adjust the timing of a memory module you can use the appropriate `-Mx` option.

> Note if you adjust the main memory module to a setting that the memory can not support, your system may lock.  To resolve simple reset and reboot

If you run your eZ80 on a higher overclocked frequency, the firmware, on boot, will appropriately adjust the timing to ensure the memory still operates - it will typically choose wait states that may be conservative for your system.

Timing adjustments are not persisted on reboots.

The example below, adjust the timing applied to the extended memory module attached to CS0 to only 1 Wait State.

```text
C>EZ80 -M0=1W
Setting Extended Memory CS0 Wait States to 1
              CPU frequency: 25000000 Hz
            Z80 Environment: eZ80's Z80 Compatibility
      On-chip Flash W/S: 1  (80ns)
        Main Memory B/C: 1  (120ns)
Extended Memory CS0 W/S: 1  (80ns)
Extended Memory CS1 W/S: 1  (80ns)
                I/O B/C: 4  (480ns)
           TMR1 Tick rate: 50 (Counter: 270173)
```

Recommended, when you adjust timings, to use the scan option (`-S`) to confirm if the memory is able to operate at your new timing.

## Difference between Bus Cycle and Wait States

The eZ80 supports two modes of accessing memory.


## Bus Cycles (B/C)

Bus Cycles follow an access pattern that for the most part is compatible with how a Z80 would access memory.

The memory access is processed over 3 separate *'states'*.  With a setting of 1 B/C, the eZ80 will use 1 system clock cycles per state - thus requiring 3 clock cycles per access.  If you select 2 B/C, then the processor will use 2 clock cycles per state, thus requiring 6 clock cycles per memory access

## Wait States (W/S)

In this mode, memory is accessed in 1 clock cycle, plus any additional configured wait states.  The onboard RAM in the eZ80 runs at 0 W/S - thus the memory is read/written to during a single system clock tick.

When W/S of 1 is applied, the CPU will wait an additional clock cycle for memory access - thus each memory access will require 2 clock cycles.
