#ifndef __TEST_PATTERN
#define __TEST_PATTERN

#include <stdint.h>

extern void test_pattern(uint8_t col_row_count, uint8_t white_colour_index);

extern uint24_t get_screen_width();
extern uint24_t get_screen_height();

#endif
