#include "print-string.h"
#include <stdint.h>

extern void uart_out(char ch);

int putchar(int ch) {
  uart_out(ch);
  return 0;
}

void print_string(const char *str) {
  char ch;
  while (ch = *str++) {
    uart_out(ch);
  }
}

char nibble_to_hex_char(const uint8_t nibble) {
  if (nibble < 10)
    return nibble + '0';

  return nibble - 10 + 'A';
}

void print_hex(const uint8_t v) {
  const uint8_t high_nibble = (v >> 4) & 0x0F;
  const uint8_t low_nibble  = v & 0x0F;

  putchar(nibble_to_hex_char(high_nibble));
  putchar(nibble_to_hex_char(low_nibble));
}

void print_hex_uint24(const uint24_t v) {
  print_hex(v >> 16 & 255);
  print_hex(v >> 8 & 255);
  print_hex(v & 255);
}

void print_uint16(const uint16_t v) {
  // uint16_t max is 65535, so 5 digits + null terminator is sufficient
  char     buffer[6];
  int      i    = 0;
  uint16_t temp = v;

  // Handle the zero case explicitly
  if (temp == 0) {
    putchar('0');
    return;
  }

  // Extract digits in reverse order
  while (temp > 0) {
    buffer[i++] = (temp % 10) + '0';
    temp /= 10;
  }

  // Print the buffer in reverse to restore correct order
  while (i > 0) {
    putchar(buffer[--i]);
  }
}
