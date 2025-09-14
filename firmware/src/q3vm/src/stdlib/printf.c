#include <host-functions.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int printf(const char *fmt, ...) {
  va_list argptr;
  char    text[81];

  va_start(argptr, fmt);
  vsprintf(text, fmt, argptr);
  va_end(argptr);

  print_string(text);
  return strlen(text);
}
