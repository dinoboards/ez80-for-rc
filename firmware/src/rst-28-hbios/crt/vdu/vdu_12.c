#include "../rst-28-vars.h"
#include <v99x8.h>
#include <vdu-functions.h>
#include <vdu-types.h>

/*
// VDU 12
VDU 12 clears either the current text viewport (by default or after a VDU 4
command) or the current graphics viewport (after a VDU 5 command) to the current
text or graphics background colour respectively. In addition the text or
graphics cursor is moved to its home position (see VDU 3)
*/

void vdu_cls(void) {
  vdu_vars_t *const vdu = &hbios_vars->vdu;

  vdp_cmd_logical_move_vdp_to_vram(vdu->tviewport.left, vdu->tviewport.top, vdu->tviewport.right - vdu->tviewport.left + 8,
                                   vdu->tviewport.bottom - vdu->tviewport.top + 8, vdu->current_tbg_colour, 0, 0);

  vdu->current_tpos.x = vdu->tviewport.left;
  vdu->current_tpos.y = vdu->tviewport.top;
}
