#include "vdu.h"
#include <v99x8.h>

// VDU: 16 (0 bytes)
void vdu_clg() {
  // for moment lets just erase to black
  // TODO: apply correct back colour

  int16_t        left   = convert_x(gviewport.left);
  int16_t        right  = convert_x(gviewport.right);
  int16_t        top    = convert_y(gviewport.top);
  int16_t        bottom = convert_y(gviewport.bottom);
  const uint16_t width  = right - left + 1;
  const uint16_t height = bottom - top + 1;

  vdp_cmd_wait_completion();
  vdp_cmd_logical_move_vdp_to_vram(left, top, width, height, 0, 0, 0);

  current_gpos.x = 0;
  current_gpos.y = 0;
}
