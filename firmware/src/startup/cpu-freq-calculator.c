#include <stdint.h>

extern uint32_t cpu_freq_calculated;
extern uint8_t  ticks_per_10_us;

// Factor for evaluation loop, where on-chip rom has 4 wait states
#define TIMER_FACTOR (370)

#define UPPER_FOR_40MHZ ((40000000 / TIMER_FACTOR) + 64)
#define LOWER_FOR_40MHZ (UPPER_FOR_40MHZ - 1024)
#define UPPER_FOR_35MHZ ((35000000 / TIMER_FACTOR) + 64)
#define LOWER_FOR_35MHZ (UPPER_FOR_35MHZ - 1024)
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

  else if (de >= LOWER_FOR_35MHZ && de <= UPPER_FOR_35MHZ)
    cpu_freq_calculated = 35000000;

  else if (de >= LOWER_FOR_40MHZ && de <= UPPER_FOR_40MHZ)
    cpu_freq_calculated = 40000000;

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

static uint24_t bus_cycle_to_ns(const uint8_t bc, const uint24_t cpu_mhz) { return ((uint24_t)bc * 3000) / cpu_mhz; }

static uint24_t ws_to_ns(const uint8_t ws, const uint24_t cpu_mhz) { return (((uint24_t)ws + 1) * 1000) / cpu_mhz; }

// Min state
// minimum_states - top most bit, if set, must return as Bus Cycle
uint8_t calculate_wait_state(const uint24_t min_nanoseconds, uint24_t minimum_states /* only 8 bit value accepted */) {
  const uint24_t cpu_mhz    = cpu_freq_calculated / 1000000;
  const uint8_t  must_be_BC = minimum_states & 0x80;
  uint8_t        state;

  if (must_be_BC) {
    for (state = minimum_states & 0x7F; state <= 15; state++) {
      if (bus_cycle_to_ns(state, cpu_mhz) >= min_nanoseconds)
        return state + 0x80;
    }

    return 0x8F;
  }

  for (state = 0; state <= 7; state++) {
    uint24_t t = ws_to_ns(state, cpu_mhz);
    if (t >= min_nanoseconds) {
      goto finish;
    }
  }

  state = 7;

finish:
  return state;
}

uint8_t calculate_emulated_io_clock_rate() { return cpu_freq_calculated / 670000; }

uint16_t calculate_default_led_flash_clock_rate() {
  // once every 1/4 seconds

  return cpu_freq_calculated / (256 * 4096 * 4);
}
