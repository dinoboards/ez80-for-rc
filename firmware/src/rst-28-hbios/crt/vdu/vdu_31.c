#include "../rst-28-vars.h"
#include "../vdu.h"

void vdu_tab(void) {
  vdu_vars_t *const vdu = &hbios_vars->vdu;

  vdu->current_tpos.x = (int24_t)vdu->data[0] * 8;
  vdu->current_tpos.y = (int24_t)vdu->data[1] * 8;
}
