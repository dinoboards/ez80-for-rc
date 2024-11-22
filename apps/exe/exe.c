#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

extern int main_func(int argc, const char **argv);

extern const uint8_t start_marshalling[];
extern const uint8_t end_marshalling[];

uint8_t *const marshalling_vectors = (uint8_t *)(0x200000);

int main(const int argc, const char *argv[]) {
  size_t n;

  if (argc < 2) {
    printf("Usage: %s <filename> <...arguments>\r\n", argv[0]);
    return 1;
  }

  const char *filename = argv[1];
  uint8_t    *ptr      = (uint8_t *)0x200400;

  FILE *f = fopen(filename, "r");
  if (f == NULL) {
    printf("Failed to open file %s\r\n", filename);
    return 1;
  }

  do {
    n = fread(ptr, 1, 512, f);
    ptr += 512;
  } while (n > 0);

  fclose(f);

  memcpy(marshalling_vectors, start_marshalling, end_marshalling - start_marshalling);

  return main_func(argc - 1, &argv[1]);
}
