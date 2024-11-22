#include <errno.h>
#include <stdint.h>
#include <stdio.h>

int main(const int argc, const char *argv[]) {
  if (argc != 2) {
    printf("Usage: %s <filename>\r\n", argv[0]);
    return 1;
  }

  const char *filename = argv[1];
  uint8_t    *ptr      = (uint8_t *)0x200000;

  FILE *f = fopen(filename, "r");
  printf("fopen: %p\r\n", f);
  if (f == NULL) {
    printf("fopen: %p, (errno: %d) FAIL\r\n", f, errno);
    return 1;
  }

  size_t n;

  do {
    n = fread(ptr, 1, 512, f);
    ptr += 512;

    printf("fread to %p: %d, (errno: %d) OK\r\n", ptr, n, errno);
  } while (n > 0);

  int r = fclose(f);
  printf("fclose: %d, (errno: %d) OK\r\n", r, errno);

  return 0;
}
