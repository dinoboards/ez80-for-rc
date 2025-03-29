#include "ez80.h"
#include <ez80-firmware.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern uint8_t ez80_mem1_bus_cycles_setxx(const uint8_t cycles);

void config_mem1(mem_config_t mem_config) {
  if (mem_config.type == WAIT_STATE) {
    printf("Setting Extended Memory CS1 Wait States to %d\r\n", mem_config.value);
    ez80_mem1_bus_cycles_setxx(mem_config.value);

    return;
  }

  if (mem_config.type == BUS_CYCLE) {
    printf("Setting Extended Memory CS1 Bus Cycles to %d\r\n", mem_config.value);
    ez80_mem1_bus_cycles_setxx(mem_config.value | 0x80);

    return;
  }
}
