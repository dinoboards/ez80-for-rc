#include "rom-flashing-calculator.h"
#include <stdint.h>
#include <stdlib.h>

#define MIN_DURATION     510
#define MAX_DURATION     650
#define OPTIMAL_DURATION (((MAX_DURATION - MIN_DURATION) / 2) + MIN_DURATION)

extern uint32_t cpu_freq_calculated;

// TODO: this function is overkill, just do
//  return CEILING [cpu_freq_calculated x (5.1/1000000)]

uint8_t calculate_fdiv(void) {
  uint32_t optimal_duration = 100000;
  uint8_t  optimal_fdiv     = 0;
  uint32_t duration_diff;
  uint8_t  i;

  for (i = 1; i < 255; i++) {
    uint32_t duration = 100000000 / (cpu_freq_calculated / i);

    if (duration > MIN_DURATION && duration < MAX_DURATION) {

      uint32_t duration_diff = abs(duration - OPTIMAL_DURATION);
      if (duration_diff < optimal_duration) {
        optimal_duration = duration_diff;
        optimal_fdiv     = i;
      }
    }
  }

  return optimal_fdiv;
}
