#include "../rst-28-vars.h"
#include <v99x8.h>
#include <vdu-functions.h>

// VDU: 16 (0 bytes)
void vdu_clg(void) {
  vdu_vars_t *const vdu = &hbios_vars->vdu;

  // for moment lets just erase to black
  // TODO: apply correct back colour

  const int24_t left = convert_x(vdu->gviewport.left);
  const int24_t top  = convert_y(vdu->gviewport.top);

  vdp_cmd_logical_move_vdp_to_vram(left, top, convert_x(vdu->gviewport.right) - left + 1,
                                   convert_y(vdu->gviewport.bottom) - top + 1, 0, 0, 0);

  vdu->current_gpos.x = 0;
  vdu->current_gpos.y = 0;
}
