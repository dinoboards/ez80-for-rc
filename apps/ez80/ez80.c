#include "ez80.h"
#include "ez80-firmware.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void show_help() {
  printf("Usage: ez80 [options]\r\n");
  printf("Options:\r\n");
  printf("  -M0=number[W|B]  Set Extended Memory Wait States or Bus Cycles (CS0)\r\n");
  printf("                   Wait States: 0-7, Bus Cycles: 1-15\r\n");
  printf("  -S0              Scan extended memory\r\n");
  printf("  -? /?            Show this help message\r\n");
}

int main(const int argc, const char *argv[]) {
  bool mem0_set = false;
  bool scan_ext_mem_set = false;
  const char* mem0_value = NULL;

  if (argc == 1) {
    report_memory_timing();
    return 0;
  }

  // First loop: Validate parameters
  for (int i = 1; i < argc; i++) {
    if (strncmp(argv[i], "-M0=", 4) == 0 || strncmp(argv[i], "/M0=", 4) == 0) {
      if (scan_ext_mem_set) {
        printf("Error: -M0 and -S0 options are mutually exclusive.\r\n");
        show_help();
        return 1;
      }
      mem0_set = true;
      mem0_value = argv[i] + 4;

    } else if (strcmp(argv[i], "-S0") == 0 || strcmp(argv[i], "/S0") == 0 || strcmp(argv[i], "-s") == 0) {
      if (mem0_set) {
        printf("Error: -M0 and -S0 options are mutually exclusive.\r\n");
        show_help();
        return 1;
      }
      scan_ext_mem_set = true;

    } else if (strcmp(argv[i], "-?") == 0 || strcmp(argv[i], "/?") == 0) {
      show_help();
      return 0;

    } else if (strcmp(argv[i], "-?") != 0 && strcmp(argv[i], "/?") != 0) {
      printf("Unknown argument: %s\r\n", argv[i]);
      show_help();
      return 1;
    }
  }

  if(mem0_set) {
    config_mem0(mem0_value);

  } else if (scan_ext_mem_set) {
    find_extended_memory();
  }

  return 0;
}
