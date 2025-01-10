#include <stdint.h>

extern uint32_t cpu_freq_calculated;
extern uint8_t  ticks_per_10_us;

// Factor for evaluation loop, where on-chip rom has 4 wait states
#define TIMER_FACTOR (270)

#define UPPER_FOR_32MHZ ((32000000 / TIMER_FACTOR) + 64)
#define LOWER_FOR_32MHZ (UPPER_FOR_32MHZ - 1024)
#define UPPER_FOR_25MHZ ((25000000 / TIMER_FACTOR) + 64)
#define LOWER_FOR_25MHZ (UPPER_FOR_25MHZ - 1024)
#define UPPER_FOR_24MHZ ((24000000 / TIMER_FACTOR) + 64)
#define LOWER_FOR_24MHZ (UPPER_FOR_24MHZ - 1024)
#define UPPER_FOR_20MHZ ((20000000 / TIMER_FACTOR) + 64)
#define LOWER_FOR_20MHZ (UPPER_FOR_20MHZ - 1024)
#define UPPER_FOR_18MHZ ((18432000 / TIMER_FACTOR) + 64)
#define LOWER_FOR_18MHZ (UPPER_FOR_18MHZ - 1024)
#define UPPER_FOR_16MHZ ((16000000 / TIMER_FACTOR) + 64)
#define LOWER_FOR_16MHZ (UPPER_FOR_16MHZ - 1024)
#define UPPER_FOR_14MHZ ((14745600 / TIMER_FACTOR) + 64)
#define LOWER_FOR_14MHZ (UPPER_FOR_14MHZ - 1024)
#define UPPER_FOR_7MHZ  ((7372800 / TIMER_FACTOR) + 64)
#define LOWER_FOR_7MHZ  (UPPER_FOR_7MHZ - 1024)

void assign_cpu_frequency(const unsigned de) {
  cpu_freq_calculated = ((int32_t)de * (int32_t)TIMER_FACTOR) / 100L * 100L;

  if (de >= LOWER_FOR_7MHZ && de <= UPPER_FOR_7MHZ)
    cpu_freq_calculated = 7372800;

  else if (de >= LOWER_FOR_14MHZ && de <= UPPER_FOR_14MHZ)
    cpu_freq_calculated = 14745600;

  else if (de > LOWER_FOR_16MHZ && de <= UPPER_FOR_16MHZ)
    cpu_freq_calculated = 16000000;

  else if (de > LOWER_FOR_18MHZ && de <= UPPER_FOR_18MHZ)
    cpu_freq_calculated = 18432000;

  else if (de >= LOWER_FOR_20MHZ && de <= UPPER_FOR_20MHZ)
    cpu_freq_calculated = 20000000;

  else if (de >= LOWER_FOR_24MHZ && de <= UPPER_FOR_24MHZ)
    cpu_freq_calculated = 24000000;

  else if (de >= LOWER_FOR_25MHZ && de <= UPPER_FOR_25MHZ)
    cpu_freq_calculated = 25000000;

  else if (de >= LOWER_FOR_32MHZ && de <= UPPER_FOR_32MHZ)
    cpu_freq_calculated = 32000000;

  ticks_per_10_us = cpu_freq_calculated / 1600000;
}

//
// calculate the reload value for a div/4 timer
// such that it triggers/loops after approx 5us
// see EZ80_DELAY (firmware_rst_18_hook)
//
#define OVERHEAD_OFFSET 13
uint8_t calculate_tmr0_rr() {
  int16_t result = (cpu_freq_calculated / 800000) - OVERHEAD_OFFSET;

  if (result <= 0)
    return 1;

  return (uint8_t)result;
}

uint8_t calculate_wait_state(const uint24_t min_nanoseconds, uint24_t minimum_wait_states /* only 8 bit value accepted */) {
  uint8_t  bc;
  uint24_t ws;
  uint24_t duration_kns;

  uint24_t cpu_freq_khz     = cpu_freq_calculated / 1000;
  uint24_t ns_per_cycle     = 1000000 / cpu_freq_khz;
  uint24_t min_duration_kns = (min_nanoseconds * 1000);
  minimum_wait_states       = (minimum_wait_states & 0xFF) * 1000;

  for (ws = 500; ws < 44500; ws += 1000) {
    duration_kns = (ns_per_cycle * ws);

    if (duration_kns >= min_duration_kns)
      break;
  }

  if (ws <= minimum_wait_states)
    ws = (minimum_wait_states);

  // Must use Z80 BUS CYCLES to get sufficient wait states
  bc = (ws / 2000) + 1;

  if (bc > 15)
    bc = 15;

  return bc | 0x80; // OR in Z80/BUS CYCLE mode flag
}
