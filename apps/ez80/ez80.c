#include "ez80-firmware.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

int main(/*const int argc, const char *argv[]*/) {
  uint32_t cpu_frequency;
  uint8_t  external_mem_bus_cycles;
  uint8_t  external_io_bus_cycles;

  cpu_frequency = ez80_cpu_freq_get();

  printf("CPU frequency: %ld Hz\r\n", (cpu_frequency));

  external_mem_bus_cycles = ez80_mem_bus_cycles_get();
  external_io_bus_cycles  = ez80_io_bus_cycles_get();

  bool mem_bus_mode = external_mem_bus_cycles & 0x80;
  bool io_bus_mode  = external_io_bus_cycles & 0x80;
  external_mem_bus_cycles &= 0x7F;
  external_io_bus_cycles &= 0x7F;

  printf("Memory bus mode: %s\r\n", mem_bus_mode ? "Z80" : "eZ80");
  printf("I/O bus mode:    %s\r\n", io_bus_mode ? "Z80" : "eZ80");

  printf("External memory bus cycles: %u\r\n", external_mem_bus_cycles);
  printf("External I/O bus cycles:    %u\r\n", external_io_bus_cycles);

  return 0;
}
