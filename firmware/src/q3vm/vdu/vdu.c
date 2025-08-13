#include "../includes/stdint.h"

extern void print_string(const char *str);

void vdu(uint8_t code) {
  char str[] = "  THIS IS THE TEST\n\r";

  str[0] = code;

  print_string(str);
}
