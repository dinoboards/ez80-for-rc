#include "rom-flashing-calculator.h"
#include <stdint.h>
#include <stdlib.h>

extern uint32_t cpu_freq_calculated;

uint8_t calculate_fdiv(void) { return (uint8_t)((((cpu_freq_calculated * 52) / 100000L) + 50L) / 100L); }
