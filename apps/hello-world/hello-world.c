#include <stdint.h>
#include <stdio.h>

char input[4];

int main(void) {
  uint32_t a = 1;
  uint32_t b = 2;
  uint32_t c = a + b;

  printf("Hello, World! 1+2 is %d\r\n", c);

  fgets(input, sizeof(input), stdin);
  printf("\r\n");

  printf("You entered: %s\r\n", input);

  return 1;
}
