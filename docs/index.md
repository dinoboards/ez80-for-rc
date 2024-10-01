---
layout: default
---


The *eZ80 for RC* is a CPU Module designed for the RCBus and RC2014<strong>&trade;</strong> backplanes.

The eZ80 Zilog CPU is an updated version of Z80 CPU. It comes in a few variations with many on chip facilities in addition to the basic CPU, such as flash ROM, RAM, GPIO and other IO services.

<div style="text-align: center;">
  <img src="./assets/eZ80-V1.7-installed-profile-front.jpg" alt="eZ80 on RC2014" width="50%">
</div>

## eZ80 CPU

The eZ80 CPU is an enhanced version of the original Z80, largely maintaining compatibility with its predecessor while offering significant improvements. It can directly address up to 16MB of memory and achieves far greater performance per clock cycle due to increased instruction efficiency and pipelining. Additionally, it integrates various on-chip peripherals such as timers, UART, SPI, I2C, and GPIOs, reducing the need for external hardware components.

While the eZ80 might appear slow and limited by today's standards, it represents, I think, an interesting evolution from the original 1970s Z80 to the modern day embedded microcontrollers.

See the Wikipedia page for basic overview of the CPU (<a href="https://en.wikipedia.org/wiki/Zilog_eZ80">https://en.wikipedia.org/wiki/Zilog_eZ80</a>)

## Project Goals

<ul>
  <li>Maximize compatibility with existing RCBus and RC2014<strong>&trade;</strong> retro modules.</li>
  <li>Create a fun DIY kit that can be assembled by anyone with basic soldering skills.</li>
  <li>Maintain the retro atheistic of the RCBus and RC2014<strong>&trade;</strong> modules.</li>
  <li>Port RomWBW/HBIOS and ensure compatibility with existing software (HBIOS, CP/M, BASIC, etc).</li>
  <li>Make it as easy as possible for builders to do their own tinkering and hacking of the eZ80.</li>
  <li>Develop a new Memory Module designed specifically for the <i>eZ80 for RC</i> to allow it to use it large linear memory access.</li>
</ul>

## Schematics

* [eZ80 Interface Module](./assets/SCHEMATIC-EZ80-INTERFACE-2024-08-02.pdf)
* [ez80 CPU Module](./assets/SCHEMATIC-EZ80-2024-08-02.pdf)
