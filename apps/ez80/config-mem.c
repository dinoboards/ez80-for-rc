#include "ez80.h"
#include <ez80-firmware.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void config_mem(mem_config_t mem_config) {
  if (mem_config.type == WAIT_STATE) {
    printf("Setting Main Segment Memory Wait States to %d\r\n", mem_config.value);
    ez80_mem_bus_cycles_set(mem_config.value);

    return;
  }

  if (mem_config.type == BUS_CYCLE) {
    printf("Setting Main Segment Memory Bus Cycles to %d\r\n", mem_config.value);
    ez80_mem_bus_cycles_set(mem_config.value | 0x80);

    return;
  }
}
