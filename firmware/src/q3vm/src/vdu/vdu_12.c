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
  const uint24_t width  = sm.tviewport.right - sm.tviewport.left + 8;
  const uint24_t height = sm.tviewport.bottom - sm.tviewport.top + 8;

  vdp_cmd_wait_completion();
  vdp_cmd_logical_move_vdp_to_vram(sm.tviewport.left, sm.tviewport.top, width, height, sm.current_tbg_colour, 0, 0);

  current_tpos.x = sm.tviewport.left;
  current_tpos.y = sm.tviewport.top;
}
