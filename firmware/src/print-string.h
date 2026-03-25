#ifndef __PRINT_STRING__
#define __PRINT_STRING__

#include <stdint.h>

int putchar(int ch);

void print_string(const char *str);
void print_hex(const uint8_t v);
void print_uint16(const uint16_t v);
void print_hex_uint24(const uint24_t v);

#endif
