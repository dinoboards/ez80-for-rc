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

  if (argc == 1) {
    report_memory_timing();
    return 0;
  }

  for (int i = 1; i < argc; i++) {
    if (strncmp(argv[i], "-M0=", 4) == 0 || strncmp(argv[i], "/M0=", 4) == 0) {
      config_mem0(argv[i] + 4);

    } else if (strcmp(argv[i], "-S0") == 0 || strcmp(argv[i], "/S0") == 0) {
      find_extended_memory();

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
