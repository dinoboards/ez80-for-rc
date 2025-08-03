#ifndef __CRT
#define __CRT

#include <stdint.h>

extern const uint8_t sysfont[(128 - ' ') * 8]; // 96*8

extern void crt_font_set_colours(const uint8_t background_colour, const uint8_t foreground_colour);

extern void crt_font_8x8_clear_patterns();
extern void crt_font_8x8_load_patterns(const uint8_t *const font_patterns, const uint8_t start_ch, const uint8_t number_of_ch);

extern void crt_font_8x8_print(const char ch, const uint16_t x, const uint16_t y);

#endif
