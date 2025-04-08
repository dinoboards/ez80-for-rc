#include "ez80.h"
#include <ez80-firmware.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

uint32_t cpu_frequency;
uint24_t flash_ns;
uint24_t main_ns;
uint24_t cs0_ns;
uint24_t cs1_ns;
uint24_t io_ns;

uint24_t mem_bus_cycles;
uint8_t  flash_wait_cycles;
uint8_t  main_mem_bus_cycles;
uint8_t  extended_mem0_bus_cycles;
uint8_t  extended_mem1_bus_cycles;
uint8_t  io_bus_cycles;
bool     mem_bus_mode;
bool     ext_mem0_bus_mode;
bool     ext_mem1_bus_mode;
bool     io_bus_mode;

static uint24_t bus_cycle_to_ns(uint8_t bc) { return ((uint32_t)bc * 3000000000) / cpu_frequency; }

static uint24_t ws_to_ns(uint8_t ws) { return (((uint32_t)ws + 1) * 1000000000) / cpu_frequency; }

void find_ns_timing_for_memory() {
  cpu_frequency     = ez80_cpu_freq_get();
  mem_bus_cycles    = ez80_mem_bus_cycles_get();
  flash_wait_cycles = ez80_flash_wait_cycles_get();
  io_bus_cycles     = ez80_io_bus_cycles_get();

  main_mem_bus_cycles      = mem_bus_cycles & 0xFF;
  extended_mem0_bus_cycles = (mem_bus_cycles >> 8) & 0xFF;
  extended_mem1_bus_cycles = (mem_bus_cycles >> 16) & 0xFF;
  mem_bus_mode             = main_mem_bus_cycles & 0x80;
  ext_mem0_bus_mode        = extended_mem0_bus_cycles & 0x80;
  ext_mem1_bus_mode        = extended_mem1_bus_cycles & 0x80;
  io_bus_mode              = io_bus_cycles & 0x80;
  main_mem_bus_cycles &= 0x7F;
  extended_mem0_bus_cycles &= 0x7F;
  extended_mem1_bus_cycles &= 0x7F;
  io_bus_cycles &= 0x7F;

  flash_ns = ws_to_ns(flash_wait_cycles);

  if (mem_bus_mode)
    main_ns = bus_cycle_to_ns(main_mem_bus_cycles);
  else
    main_ns = ws_to_ns(main_mem_bus_cycles);

  if (ext_mem0_bus_mode)
    cs0_ns = bus_cycle_to_ns(extended_mem0_bus_cycles);
  else
    cs0_ns = ws_to_ns(extended_mem0_bus_cycles);

  if (ext_mem1_bus_mode)
    cs1_ns = bus_cycle_to_ns(extended_mem1_bus_cycles);
  else
    cs1_ns = ws_to_ns(extended_mem1_bus_cycles);

  if (io_bus_mode)
    io_ns = bus_cycle_to_ns(io_bus_cycles);
  else
    io_ns = bus_cycle_to_ns(io_bus_cycles);
}

void report_memory_timing(void) {
  find_ns_timing_for_memory();

  printf("              CPU frequency: %ld Hz\r\n", (cpu_frequency));

  printf("      On-chip Flash W/S: %u  (%dns)\r\n", flash_wait_cycles, flash_ns);

  if (mem_bus_mode) {
    printf("        Main Memory B/C: %u  (%dns)\r\n", main_mem_bus_cycles, main_ns);
  } else {
    printf("        Main Memory W/S: %u  (%dns)\r\n", main_mem_bus_cycles, main_ns);
  }

  if (ext_mem0_bus_mode) {
    printf("Extended Memory CS0 B/C: %u  (%dns)\r\n", extended_mem0_bus_cycles, cs0_ns);
  } else {
    printf("Extended Memory CS0 W/S: %u  (%dns)\r\n", extended_mem0_bus_cycles, cs0_ns);
  }

  if (ext_mem1_bus_mode) {
    printf("Extended Memory CS1 B/C: %u  (%dns)\r\n", extended_mem1_bus_cycles, cs1_ns);
  } else {
    printf("Extended Memory CS1 W/S: %u  (%dns)\r\n", extended_mem1_bus_cycles, cs1_ns);
  }

  if (io_bus_mode) {
    printf("                I/O B/C: %u  (%dns)\r\n", io_bus_cycles, io_ns);
  } else {
    printf("                I/O W/S: %u  (%dns)\r\n", io_bus_cycles, io_ns);
  }

  printf("           TMR1 Tick rate: %d (Counter: %u)\r\n", ez80_timers_freq_tick_get(), ez80_timers_ticks_get());
}
