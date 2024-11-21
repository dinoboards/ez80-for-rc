#include "ez80-firmware.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void report_status(void) {
  uint32_t cpu_frequency = ez80_cpu_freq_get();

  printf("              CPU frequency: %ld Hz\r\n", (cpu_frequency));

  uint24_t mem_bus_cycles          = ez80_mem_bus_cycles_get();
  uint8_t  main_mem_bus_cycles     = mem_bus_cycles & 0xFF;
  uint8_t  extended_mem_bus_cycles = (mem_bus_cycles >> 8) & 0xFF;

  uint8_t io_bus_cycles = ez80_io_bus_cycles_get();

  bool mem_bus_mode     = main_mem_bus_cycles & 0x80;
  bool ext_mem_bus_mode = extended_mem_bus_cycles & 0x80;
  bool io_bus_mode      = io_bus_cycles & 0x80;
  main_mem_bus_cycles &= 0x7F;
  extended_mem_bus_cycles &= 0x7F;
  io_bus_cycles &= 0x7F;

  if (mem_bus_mode) {
    printf("     Main Memory Bus Cycles: %u\r\n", main_mem_bus_cycles);
  } else {
    printf("    Main Memory Wait States: %u\r\n", main_mem_bus_cycles);
  }

  if (ext_mem_bus_mode) {
    printf(" Extended Memory Bus Cycles: %u\r\n", extended_mem_bus_cycles);
  } else {
    printf("Extended Memory Wait States: %u\r\n", extended_mem_bus_cycles);
  }

  if (io_bus_mode) {
    printf("             I/O Bus Cycles: %u\r\n", io_bus_cycles);
  } else {
    printf("            I/O Wait States: %u\r\n", io_bus_cycles);
  }
}

void show_help() {
  printf("Usage: ez80 [options]\r\n");
  printf("Options:\r\n");
  printf("  -M0=number[W|B]  Set Extended Memory Wait States or Bus Cycles (CS0)\r\n");
  printf("                   Wait States: 0-7, Bus Cycles: 1-15\r\n");
  printf("  -? /?            Show this help message\r\n");
}

void process_mem0(const char *value) {
  int number = atoi(value);
  char type = value[strlen(value) - 1];

  if (type == 'W') {
    if (number >= 0 && number <= 7) {
      printf("Setting Main Memory Wait States to %d\r\n", number);
      // Set the wait state value here
    } else {
      printf("Invalid wait state value for -M0: %s\r\n\n", value);
      show_help();
    }
  } else if (type == 'B') {
    if (number >= 1 && number <= 15) {
      printf("Setting Main Memory Bus Cycles to %d\r\n", number);
      ez80_mem0_bus_cycles_set(number);
      // Set the bus cycle value here
    } else {
      printf("Invalid bus cycle value for -M0: %s\r\n\n", value);
      show_help();
    }
  } else {
    printf("Invalid value for -M0: %s\r\n\n", value);
    show_help();
  }
}

int main(const int argc, const char *argv[]) {

  if (argc == 1) {
    report_status();
    return 0;
  }

  for (int i = 1; i < argc; i++) {
    if (strncmp(argv[i], "-M0=", 4) == 0 || strncmp(argv[i], "/M0=", 4) == 0) {
      process_mem0(argv[i] + 4);
    } else if (strcmp(argv[i], "-?") == 0 || strcmp(argv[i], "/?") == 0) {
      show_help();
      return 0;
    } else {
      printf("Unknown argument: %s\n", argv[i]);
      show_help();
      return 1;
    }
  }


  return 0;
}
