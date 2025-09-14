#include <string.h>

size_t strlen(const char *string) {
  const char *s;

  s = string;
  while (*s) {
    s++;
  }
  return s - string;
}
