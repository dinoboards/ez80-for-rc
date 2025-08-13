#include <host-functions.h>
#include <stdint.h>

void vdu(uint8_t code) { trap_Printf("Char %d %c\r\n\n"); }
