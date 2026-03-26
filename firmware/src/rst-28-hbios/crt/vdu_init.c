#include "../rst-28-vars.h"
#include "vdu.h"
#include "vdu/vdu_consts.h"
#include <stdint.h>

void vdu_init(void) {
  vdu_vars_t *const vdu = &hbios_vars->vdu;

  init_font_patterns();

  vdu->gviewport.left   = 0;
  vdu->gviewport.bottom = 0;
  vdu->gviewport.right  = scale_width - 1;
  vdu->gviewport.top    = scale_height - 1;
}
