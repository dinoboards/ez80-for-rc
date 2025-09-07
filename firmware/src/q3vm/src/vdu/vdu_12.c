#include <v99x8.h>
#include <vdu-types.h>
#include <vm-shared-mem.h>

/*
// VDU 12
VDU 12 clears either the current text viewport (by default or after a VDU 4
command) or the current graphics viewport (after a VDU 5 command) to the current
text or graphics background colour respectively. In addition the text or
graphics cursor is moved to its home position (see VDU 3)
*/

void vdu_cls(void) {
  const uint24_t width  = tviewport.right - tviewport.left + 8;
  const uint24_t height = tviewport.bottom - tviewport.top + 8;

  vdp_cmd_logical_move_vdp_to_vram(tviewport.left, tviewport.top, width, height, current_tbg_colour, 0, 0);

  current_tpos.x = tviewport.left;
  current_tpos.y = tviewport.top;
}
