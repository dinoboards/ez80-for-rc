#include "bg_lib.h"
#include <host-functions.h>
#include <stdint.h>

void vdu(uint24_t code) {
  printf("Char is %d\n\r", code);
}
