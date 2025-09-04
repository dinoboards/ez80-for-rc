#include "../vdu.h"
#include <stdint.h>
#include <v99x8.h>

void vdu_lf(void) {

  if (current_tpos.y < sm.tviewport.bottom) {
    current_tpos.y += 8;
  } else {

    uint24_t left   = sm.tviewport.left;
    uint24_t top    = sm.tviewport.top;
    uint24_t right  = sm.tviewport.right;
    uint24_t bottom = sm.tviewport.bottom;

    uint24_t width  = right - left + 8;
    uint24_t height = bottom - top;

    // TODO: ONLY SCROLL IF TEXT CURSOR IS ACTIVE

    vdp_cmd_wait_completion();
    vdp_cmd_move_vram_to_vram(left, top + 8, left, top, width, height, 0);
    vdp_cmd_wait_completion();
    vdp_cmd_vdp_to_vram(left, bottom, width, 8, sm.current_tbg_colour, 0);
  }
}
