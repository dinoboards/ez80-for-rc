#include "../vdu.h"
#include <v99x8.h>

/*
// VDU 12
VDU 12 clears either the current text viewport (by default or after a VDU 4
command) or the current graphics viewport (after a VDU 5 command) to the current
text or graphics background colour respectively. In addition the text or
graphics cursor is moved to its home position (see VDU 3)
*/

void vdu_cls() {
  // for moment lets just erase to black
  // TODO constrain to text view port
  // apply correct back colour

  const uint24_t left   = tviewport.left;
  const uint24_t bottom = tviewport.bottom;
  const uint24_t right  = tviewport.right;
  const uint24_t top    = tviewport.top;

  const uint24_t width  = right - left + 8;
  const uint24_t height = bottom - top + 8;

  vdp_cmd_wait_completion();
  vdp_cmd_logical_move_vdp_to_vram(left, top, width, height, current_tbg_colour, 0, 0);

  current_tpos.x = tviewport.left;
  current_tpos.y = tviewport.top;
}
