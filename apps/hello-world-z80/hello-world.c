#pragma printf = "%0X %X %x %s %c %u %f %d %u %ld %lld %llu %lu %p"

#include <stdint.h>
#include <stdio.h>

char input[4];

int main(void) {
  uint16_t a = 1;
  uint16_t b = 2;
  uint16_t c = a + b;

  printf("Hello, World! 1+2 is %d\r\n", c);

  fgets(input, sizeof(input), stdin);
  printf("\r\n");

  printf("You entered: %s\r\n", input);

  printf("Enter a single character: ");
  const unsigned char ch = getchar();
  printf("\r\nYou entered: %c\r\n", ch);

  return 1;
}
