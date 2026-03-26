#include "variables.h"
#include <stdint.h>

// VDU: 29 (4bytes)
void vdu_set_origin(void) {
  origin.x = *((int16_t *)&data[0]);
  origin.y = *((int16_t *)&data[2]);
}
