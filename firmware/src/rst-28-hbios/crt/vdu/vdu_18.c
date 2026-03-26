#include "../rst-28-vars.h"

// VDU: 18 (2 bytes)
void vdu_gcol(void) {
  vdu_vars_t *const vdu = &hbios_vars->vdu;

  vdu->current_operation_mode = vdu->data[0];
  vdu->current_gfg_colour     = vdu->data[1];
}
