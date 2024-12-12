#include "ez80.h"
#include <ez80-firmware.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void config_flash(mem_config_t mem_config) {
  printf("Setting On-chip Flash Memory Wait States to %d\r\n", mem_config.value);
  ez80_flash_wait_cycles_set(mem_config.value);
}
