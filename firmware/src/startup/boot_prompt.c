#include <stdint.h>

extern void uart_out(char ch);

int putchar(int ch) { uart_out(ch); }

void print_string(const char *str) {
  char ch;
  while (ch = *str++) {
    uart_out(ch);
  }
}

void boot_prompt() {
  print_string("\r\n\n\neZ80 for RC Initialised.\r\n");
  print_string("Starting System...\r\n");
}
