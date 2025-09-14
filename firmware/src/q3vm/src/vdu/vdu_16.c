#include <v99x8.h>
#include <vdu-functions.h>
#include <vm-shared-mem.h>

// VDU: 16 (0 bytes)
void vdu_clg(void) {
  // for moment lets just erase to black
  // TODO: apply correct back colour

  const int24_t left = convert_x(gviewport.left);
  const int24_t top  = convert_y(gviewport.top);

  vdp_cmd_logical_move_vdp_to_vram(left, top, convert_x(gviewport.right) - left + 1, convert_y(gviewport.bottom) - top + 1, 0, 0,
                                   0);

  current_gpos.x = 0;
  current_gpos.y = 0;
}
