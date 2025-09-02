#include "../vdu.h"
#include <stdint.h>
#include <v99x8.h>

void vdu_lf(void) {
  current_tpos.y += 8;

  if (current_tpos.y >= tviewport.bottom) {
    current_tpos.y -= 8;

    {
      uint24_t left   = tviewport.left;
      uint24_t top    = tviewport.top;
      uint24_t right  = tviewport.right;
      uint24_t bottom = tviewport.bottom;

      uint24_t width  = right - left + 8;
      uint24_t height = bottom - top;

      // TODO: ONLY SCROLL IF TEXT CURSOR IS ACTIVE

      vdp_cmd_wait_completion();
      vdp_cmd_move_vram_to_vram(left, top + 8, left, top, width, height, 0);
      vdp_cmd_wait_completion();
      vdp_cmd_vdp_to_vram(left, bottom, width, 8, current_tbg_colour, 0);
    }
  }
}
