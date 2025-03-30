#include <ez80.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char input[4];

void do_exit() {
  printf("Exiting...\r\n");
  exit(2);
}

extern uint8_t start_of_on_chip[];
extern uint8_t end_of_on_chip[];
extern uint8_t start_of_bss_on_chip[];
extern uint8_t end_of_bss_on_chip[];
extern uint8_t on_chip_source[];
extern uint8_t length_of_on_chip[];

int foo __bss_on_chip;
int bar __data_on_chip = 42;

void __func_on_chip spike() { printf("Executing in fast on-chip ram! Bar=%d\r\n", bar); }

int main(const int argc, const char *argv[]) {
  printf("argc: %d\r\n", argc);
  for (int i = 0; i < argc; i++) {
    printf("argv[%d]: %s\r\n", i, argv[i]);
  }

  printf("extended_memory_start: %p\r\n", get_memory_start());
  printf("extended_memory_end: %p\r\n", get_memory_end());

  printf("Foo stored at %p\r\n", &foo);
  printf("bar stored at %p\r\n", &bar);
  printf("Spike function at %p\r\n", &spike);
  printf("Main function at %p\r\n", &main);

  printf("Text on chip: %p to %p\r\n", start_of_on_chip, end_of_on_chip);
  printf("BSS on chip: %p to %p\r\n", start_of_bss_on_chip, end_of_bss_on_chip);
  printf("Text on chip source: %p\r\n", on_chip_source);

  spike();

  uint24_t a = 1;
  uint24_t b = 2;
  uint24_t c = a + b;

  printf("Hello, World! 1+2 is %d\r\n", c);

  fgets(input, sizeof(input), stdin);
  printf("\r\n");

  printf("You entered: %s\r\n", input);

  printf("Enter a single character: ");
  const unsigned char ch = getchar();
  printf("\r\nYou entered: %c\r\n", ch);

  do_exit();

  printf("Did not exit - returning 1\r\b");

  return 1;
}
