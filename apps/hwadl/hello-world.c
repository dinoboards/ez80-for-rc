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

int main(const int argc, const char *argv[]) {
  printf("argc: %d\r\n", argc);
  for (int i = 0; i < argc; i++) {
    printf("argv[%d]: %s\r\n", i, argv[i]);
  }

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
