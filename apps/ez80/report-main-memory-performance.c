#include "ez80.h"
#include <ez80-firmware.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

extern uint8_t main_memory_buffer[4096];

int24_t measure_memory_performance(uint8_t *ptr);

#define BYTES_COPIED (4096 * 1024)

void report_main_memory_performance() {
  printf("Conducting performance test.  Please wait.\r\n");
  int ticks = measure_memory_performance(main_memory_buffer);

  const float kb_per_second = 4096.0f / ((float)ticks / (float)ez80_timers_freq_tick_get());

  printf("Main Memory %.2f KB/s", kb_per_second);
}

void report_extended_performance(uint8_t *ptr) {
  printf("Conducting performance test.  Please wait.\r");
  int ticks = measure_memory_performance(ptr);

  const float kb_per_second = 4096.0f / ((float)ticks / (float)ez80_timers_freq_tick_get());

  printf("Memory @ %p: %.2f KB/s                              \r\n", ptr, kb_per_second);
}
