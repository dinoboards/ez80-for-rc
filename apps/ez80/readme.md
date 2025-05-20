## eZ80 utility

The `EZ80.COM` application is an application that can adjust I/O and memory timing.

<code>
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
</code>
