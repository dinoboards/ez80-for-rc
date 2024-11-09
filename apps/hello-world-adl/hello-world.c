#include <stdint.h>
#include <stdio.h>
#include <string.h>
char input[4];

int main(void) {
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

  return 1;
}
