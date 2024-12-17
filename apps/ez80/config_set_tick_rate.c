#include <ez80-firmware.h>
#include <stdint.h>
#include <stdio.h>

void config_set_tick_rate(uint8_t rate) {
  printf("Setting TMR1 Tick rate to %d\r\n", rate);
  ez80_timers_freq_tick_set(rate);
}
