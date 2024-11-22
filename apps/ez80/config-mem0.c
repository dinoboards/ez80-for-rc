#include "ez80-firmware.h"
#include "ez80.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void config_mem0(const char *value) {
  int  number = atoi(value);
  char type   = value[strlen(value) - 1];

  if (type == 'W') {
    if (number >= 0 && number <= 7) {
      printf("Setting Main Memory Wait States to %d\r\n", number);
      ez80_mem0_bus_cycles_set(number);

    } else {
      printf("Invalid wait state value for -M0: %s\r\n\n", value);
      show_help();
    }

    return;
  }

  if (type == 'B') {
    if (number >= 1 && number <= 15) {
      printf("Setting Main Memory Bus Cycles to %d\r\n", number);
      ez80_mem0_bus_cycles_set(number | 0x80);
    } else {
      printf("Invalid bus cycle value for -M0: %s\r\n\n", value);
      show_help();
    }

    return;
  }

  printf("Invalid value for -M0: %s\r\n\n", value);
  show_help();
}
