#include "vdu.h"

// VDU: 29 (4bytes)
void vdu_set_origin() {
  uint8_t *const bptr_x = (uint8_t *)&origin.x;
  uint8_t *const bptr_y = (uint8_t *)&origin.y;

  bptr_x[0]  = data[0];
  bptr_x[1]  = data[1];
  bptr_y[0]  = data[2];
  bptr_y[1]  = data[3];
  current_fn = NULL;
}
