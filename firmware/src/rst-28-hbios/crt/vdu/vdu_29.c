#include "../rst-28-vars.h"
#include <stdint.h>

// VDU: 29 (4bytes)
void vdu_set_origin(void) {
  vdu_vars_t *const vdu = &hbios_vars->vdu;

  vdu->origin.x = *((int16_t *)&vdu->data[0]);
  vdu->origin.y = *((int16_t *)&vdu->data[2]);
}
