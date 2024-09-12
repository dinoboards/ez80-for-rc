#pragma printf = "%0X %X %x %s %c %u %f %d %u %ld %lld %llu %lu %p"

#include "ez80-firmware.h"
#include <stdint.h>
#include <stdio.h>

uint8_t main(const int argc, const char *argv[]) {
  argc;
  argv;
  uint32_t cpu_frequency;
  uint16_t bus_cycles;
  uint8_t  external_mem_bus_cycles;
  uint8_t  external_io_bus_cycles;

  cpu_frequency = ez80_cpu_freq_get();

  printf("CPU frequency: %lu Hz\r\n", cpu_frequency);

  bus_cycles = ez80_bus_cycles_get();

  external_mem_bus_cycles = bus_cycles >> 8;
  external_io_bus_cycles  = bus_cycles & 0xFF;

  printf("External memory bus cycles: %u\r\n", external_mem_bus_cycles);
  printf("External I/O bus cycles: %u\r\n", external_io_bus_cycles);

  return 0;
}
