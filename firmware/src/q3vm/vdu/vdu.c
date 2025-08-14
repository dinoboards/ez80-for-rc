#include "bg_lib.h"
#include <host-functions.h>
#include <stdint.h>

void vdu(uint8_t code) { printf("Char %d %c\r\n\n", code, code); }
