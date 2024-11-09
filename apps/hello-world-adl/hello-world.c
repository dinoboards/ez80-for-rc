#include <stdint.h>
#include <stdio.h>
#include <string.h>
char input[4];

void test_strcpy() {
  char src[] = "Hello, World!";
  char dst[14];
  strcpy(dst, src);
  printf("dst: %s\r\n", dst);
}

void test_strchr() {
  char str[] = "Hello, World!";
  char *ptr = strchr(str, 'W');
  printf("ptr: %s\r\n", ptr);
}

int main(void) {
  uint24_t a = 1;
  uint24_t b = 2;
  uint24_t c = a + b;

  test_strcpy();
  test_strchr();

  printf("Hello, World! 1+2 is %d\r\n", c);

  fgets(input, sizeof(input), stdin);
  printf("\r\n");

  printf("You entered: %s\r\n", input);

  printf("Enter a single character: ");
  const unsigned char ch = getchar();
  printf("\r\nYou entered: %c\r\n", ch);

  return 1;
}
