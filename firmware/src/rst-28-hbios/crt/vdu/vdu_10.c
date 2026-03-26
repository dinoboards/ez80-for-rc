#include "../rst-28-vars.h"
#include "../vdu.h"
#include <stdint.h>
#include <v99x8.h>

void vdu_lf(void) {
  vdu_vars_t *const vdu = &hbios_vars->vdu;

  if (vdu->current_tpos.y < vdu->tviewport.bottom) {
    vdu->current_tpos.y += 8;

  } else {
    const uint24_t left   = vdu->tviewport.left;
    const uint24_t top    = vdu->tviewport.top;
    const uint24_t right  = vdu->tviewport.right;
    const uint24_t bottom = vdu->tviewport.bottom;

    const uint24_t width  = right - left + 8;
    const uint24_t height = bottom - top;

    // TODO: ONLY SCROLL IF TEXT CURSOR IS ACTIVE

    vdp_cmd_wait_completion();
    vdp_cmd_move_vram_to_vram(left, top + 8, left, top, width, height, 0);
    vdp_cmd_wait_completion();
    vdp_cmd_vdp_to_vram(left, bottom, width, 8, vdu->current_tbg_colour, 0);
  }
}
