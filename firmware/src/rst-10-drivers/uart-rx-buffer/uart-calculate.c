#include <stdint.h>

extern uint24_t baud_rate;
extern uint32_t cpu_freq_calculated;

/*
 Calculate BRG L and BRG H based on CPU CLOCK and current baud rate
 Output
   L = BRG_L
   H = BRG_H
*/

uint16_t get_brg() { return ((cpu_freq_calculated * 10 / (baud_rate * 16)) + 5) / 10; }
