---
layout: default
---

### eZ80 for RC

The *eZ80 for RC* is a CPU Module designed for the RCBus and RC2014<strong>&trade;</strong> backplanes.

The eZ80 Zilog CPU is an updated version of Z80 CPU. It comes in a few variations with many on chip facilities in addition to the basic CPU, such as flash ROM, RAM, GPIO and other IO services.

<div style="text-align: center;">
  <img src="{{ site.baseurl }}/assets/eZ80-V1.7-installed-profile-front.jpg" alt="eZ80 on RC2014" width="50%">
</div>

### What is the eZ80 CPU

The eZ80 CPU is an enhanced version of the original Z80, largely maintaining compatibility with its predecessor while offering significant improvements. It can directly address up to 16MB of memory and achieves far greater performance per clock cycle due to increased instruction efficiency and pipelining. Additionally, it integrates various on-chip peripherals such as timers, UART, SPI, I2C, and GPIOs, reducing the need for external hardware components.

While the eZ80 might appear slow and limited by today's standards, it represents, I think, an interesting evolution from the original 1970s Z80 to the modern day embedded microcontrollers.

See the Wikipedia page for basic overview of the CPU (<a href="https://en.wikipedia.org/wiki/Zilog_eZ80">https://en.wikipedia.org/wiki/Zilog_eZ80</a>)

### Key features of the eZ80 for RC

<ul>
  <li>Compatibility with many existing RCBus and RC2014<strong>&trade;</strong> retro modules.</li>
  <li>A fun DIY kit that can be assembled by anyone with basic soldering skills.</li>
  <li>Works in conjunction with RomWBW/HBIOS ensuring a high degree of compatibility with existing OS and applications (HBIOS, CP/M, BASIC, etc).</li>
  <li>Lots of potential for to do your own hacking and tinkering with the eZ80.</li>
</ul>

### Schematics

* [eZ80 Interface Module (v1.7)]({{ site.baseurl }}/assets/SCHEMATIC-EZ80-INTERFACE-V1.7.pdf)
* [ez80 CPU Module (v1.3)]({{ site.baseurl }}/assets/SCHEMATIC-EZ80-V1.3.pdf)

### Kit Details

Coming soon to my Tindie Store, will be the *eZ80 for RC* kit.  For you to assemble and expand your RC2014<strong>&trade;</strong> or RCBus system


#### What's included in the kit

The full kits includes everything you need.  Including the PCBs, capacitors, IC sockets, connectors, and the various ICs.  The included Programmable Logic Device (PLD) will also be flashed with the required logic.

The eZ80 CPU module will come with all the surface mounted components soldered.  You just need to solder the PCB pins and a standard 6 pin programming header.

A 20Mhz and an optional 25Mhz crystal to allow you to 'overclock' your eZ80.

See the Bill Of Material list for full details

### Bill Of Materials

|Quantity | Component|
|--------| -----------------------------------------|
|   2    | Right Angle 20x2 Header                  |
|   6    | 100nF                                    |
|   1    | 10uF Radial                              |
|   2    | 1uF Radial                               |
|   2    | HDR M 2.54 1x40                          |
|   2    | Shunts 1x2                               |
|   1    | 3mm Green LED                            |
|   1    | 20MHz Crystal Oscillator                 |
|   4    | 3.4mm 4.7kΩ resistor                     |
|   2    | 3.4mm 1kΩ resistor                       |
|   7    | 3.4mm 10kΩ resistor                      |
|   1    | 3.4mm 47Ω resistor                       |
|   1    | 3.4mm 470Ω resistor                      |
|   5    | 74HCT245                                 |
|   1    | ATF16V8C-7PU                             |
|   1    | TLV1117LV33DCYR                          |
|   1    | BAT-HLD-012-SMT*                         |
|   1    | MCP130-475DI/TO                          |
|   6    | 20 POS IC DIP SOCKET                     |
|   1    | 4 POS SOCKET FOR OSCILLATOR              |
|  1     | Interface PCB for RC2014/RCBus Backplane |
|  1     | pre-assembled eZ80 CPU Module            |

\* The batter holder **BAT-HLD-012-SMT** can support battery types of CR1225 or CR1216.  Please note in the current PCB the silkscreen incorrectly states a battery size of CR2016.

### Images

<div class="image-gallery">
  <div class="image-column">
    <a href="{{ site.baseurl }}/assets/pcb-interface-front.jpg" target="_blank">
      <img src="{{ site.baseurl }}/assets/pcb-interface-front.jpg" alt="eZ80 Interface PCB Top Layer">
    </a>
    <a href="{{ site.baseurl }}/assets/ez80-cpu.jpg" target="_blank">
      <img src="{{ site.baseurl }}/assets/ez80-cpu.jpg" alt="eZ80 CPU module">
    </a>
    <a href="{{ site.baseurl }}/assets/ez80-parts-wrapped.jpg" target="_blank">
      <img src="{{ site.baseurl }}/assets/ez80-parts-wrapped.jpg" alt="ez80 for rc kit parts (wrapped)">
    </a>
  </div>
  <div class="image-column">
    <a href="{{ site.baseurl }}/assets/pcb-interface-back.jpg" target="_blank">
      <img src="{{ site.baseurl }}/assets/pcb-interface-back.jpg" alt="eZ80 Interface PCB Bottom Layer">
    </a>
    <a href="{{ site.baseurl }}/assets/ez80-V1.7-installed.jpg" target="_blank">
      <img src="{{ site.baseurl }}/assets/eZ80-V1.7-installed.jpg" alt="ez80 module installed">
    </a>
    <a href="{{ site.baseurl }}/assets/ez80-kit-parts.jpg" target="_blank">
      <img src="{{ site.baseurl }}/assets/ez80-kit-parts.jpg" alt="ez80 for rc kit parts">
    </a>
  </div>
</div>


### Assembly Guide

Here are my recommendation for how to approach assembling and soldering the eZ80 Interface PCB and the CPU Module.

No surface mount soldering is needed, as all surface-mounted components are already soldered.

First I start with the PCB pins on the CPU PCB and the associated Round Machine Sockets on the main Interface PCB.  The PCB pins require careful attention, as they are quite small, and alignment is important otherwise you may have difficult inserting them into the sockets on the PCB.

<ol start="1">
<li>Cut the machine socket strips to the required lengths: 3 strips of 13 pins, 3 strips of 12 pins, and 2 strips of 9 pins. Be cautious when breaking the strips into the required lengths to avoid making a poor cut and losing or exposing the end pin. Using a sharp Stanley knife with some force can help achieve a clean break. Keep the knife straight.</li>
</ol>

<div class="image-gallery">
  <div class="image-column">
    <img src="{{ site.baseurl }}/assets/breaking-socket-strip.jpg"/>
  </div>
  <div class="image-column">
    <video style="width:100%"  controls poster="{{ site.baseurl }}/assets/demo-breaking-strip-poster.jpg">
      <source src="https://github.com/user-attachments/assets/44a49dc0-0513-4445-9710-4ada5918bb3e" type="video/mp4">
      Your browser does not support the video tag.
    </video>
  </div>
</div>

<ol start="2">
  <li>Place the sockets into the Interface PCB. Ensure the PCB is placed on foam or elevated so the sockets can sit fully in place. Do not solder them yet.</li>

  <li>Using precision tweezers, pick up one of the PCB pins and insert the shorter end into one of the sockets. Repeat this for all pins. Make sure each PCB pin is fully inserted; you should hear it click into place.</li>
</ol>

<div class="image-gallery">
  <div class="image-column">
    <img src="{{ site.baseurl }}/assets/closeup-of-pin.jpg" width="65%" style="display: block; margin-left: auto; margin-right:40px;"/>
  </div>
  <div class="image-column">
    <img src="{{ site.baseurl }}/assets/closeup-of-pins-in-socket.jpg" width="65%" style="display: block; margin-left: auto; margin-right:40px;"/>
  </div>
</div>

<div style="text-align: center;">
  <video width="400" controls poster="{{ site.baseurl }}/assets/inserting-pins-into-socket-poster.jpg">
    <source src="https://github.com/user-attachments/assets/a3bd98f9-aa68-448f-8ef9-192bd0f10790" type="video/mp4">
    Your browser does not support the video tag.
  </video>
</div>

<ol start="4">
  <li>Confirm the PCB pins are gripped within the socket - they should not fall out if the socket is turned upside down.</li>
  <li>Once all the PCB pins are inserted, position the CPU module in place.<br>
  In this video, you can see the CPU PCB and the main Interface PCB connected through the PCB pins and sockets, but they are not yet soldered.</li>
</ol>

<div style="text-align: center;">
  <video width="400" height="400" controls>
    <source src="https://github.com/user-attachments/assets/6dc4e6c8-8e58-4d5f-b291-fdaf545a1b6a" type="video/mp4">
    Your browser does not support the video tag.
  </video>
</div>

<ol start="6">
  <li>Solder the top of the CPU Module first. Then, flip it over and solder the sockets on the back of the main Interface PCB, ensuring the sockets are still fully inserted. Once both sides are soldered, remove the CPU module and continue assembling the rest of the Interface PCB.</li>
</ol>

### Disclaimer

Please note that this is a kit, produced by a non-professional (me) for hackers, DIYers' and retro lovers, to tinker with. I will do my best to answer any support questions you may have.
