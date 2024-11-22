#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void Quit(const char *errorStr) {
  printf("\r\nQuiting....\r\n");
  printf("Error: %s\r\n", errorStr);
  // char error[256];
  // if (errorStr != NULL) {
  //   va_list vlist;
  //   va_start(vlist, errorStr);
  //   vsprintf(error, errorStr, vlist);
  //   va_end(vlist);
  // }

  abort();
}
