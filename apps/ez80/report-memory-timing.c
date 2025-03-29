#include "ez80.h"
#include <ez80-firmware.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

void report_memory_timing(void) {
  uint32_t cpu_frequency = ez80_cpu_freq_get();

  printf("              CPU frequency: %ld Hz\r\n", (cpu_frequency));

  uint24_t mem_bus_cycles           = ez80_mem_bus_cycles_get();
  uint8_t  flash_wait_cycles        = ez80_flash_wait_cycles_get();
  uint8_t  main_mem_bus_cycles      = mem_bus_cycles & 0xFF;
  uint8_t  extended_mem0_bus_cycles = (mem_bus_cycles >> 8) & 0xFF;
  uint8_t  extended_mem1_bus_cycles = (mem_bus_cycles >> 16) & 0xFF;

  uint8_t io_bus_cycles = ez80_io_bus_cycles_get();

  bool mem_bus_mode      = main_mem_bus_cycles & 0x80;
  bool ext_mem0_bus_mode = extended_mem0_bus_cycles & 0x80;
  bool ext_mem1_bus_mode = extended_mem1_bus_cycles & 0x80;
  bool io_bus_mode       = io_bus_cycles & 0x80;
  main_mem_bus_cycles &= 0x7F;
  extended_mem0_bus_cycles &= 0x7F;
  extended_mem1_bus_cycles &= 0x7F;
  io_bus_cycles &= 0x7F;

  printf("      On-chip Flash W/S: %u\r\n", flash_wait_cycles);

  if (mem_bus_mode) {
    printf("        Main Memory B/C: %u\r\n", main_mem_bus_cycles);
  } else {
    printf("        Main Memory W/S: %u\r\n", main_mem_bus_cycles);
  }

  if (ext_mem0_bus_mode) {
    printf("Extended Memory CS0 B/C: %u\r\n", extended_mem0_bus_cycles);
  } else {
    printf("Extended Memory CS0 W/S: %u\r\n", extended_mem0_bus_cycles);
  }

  if (ext_mem1_bus_mode) {
    printf("Extended Memory CS1 B/C: %u\r\n", extended_mem1_bus_cycles);
  } else {
    printf("Extended Memory CS1 W/S: %u\r\n", extended_mem1_bus_cycles);
  }

  if (io_bus_mode) {
    printf("                I/O B/C: %u\r\n", io_bus_cycles);
  } else {
    printf("                I/O W/S: %u\r\n", io_bus_cycles);
  }

  printf("           TMR1 Tick rate: %d (Counter: %u)\r\n", ez80_timers_freq_tick_get(), ez80_timers_ticks_get());
}
