#include <stdint.h>
#include <vm-shared-mem.h>

// VDU: 29 (4bytes)
void vdu_set_origin(void) {
  origin.x = *((uint16_t *)&data[0]);
  origin.y = *((uint16_t *)&data[2]);
}
