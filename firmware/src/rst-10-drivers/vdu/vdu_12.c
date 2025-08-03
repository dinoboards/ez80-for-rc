#include "vdu.h"
#include <v99x8.h>

#include <stdio.h>

/*
// VDU 12
VDU 12 clears either the current text viewport (by default or after a VDU 4
command) or the current graphics viewport (after a VDU 5 command) to the current
text or graphics background colour respectively. In addition the text or
graphics cursor is moved to its home position (see VDU 3)
*/

void vdu_cls() {
  const uint16_t left   = (uint16_t)tviewport.left * 8;
  const uint16_t bottom = (uint16_t)tviewport.bottom * 8;
  const uint16_t right  = (uint16_t)tviewport.right * 8;
  const uint16_t top    = (uint16_t)tviewport.top * 8;

  const uint16_t width  = right - left + 8;
  const uint16_t height = bottom - top + 8;

  // for moment lets just erase to black
  // TODO constrain to text view port
  // apply correct back colour
  vdp_cmd_wait_completion();
  vdp_cmd_logical_move_vdp_to_vram(left, top, width, height, current_tbg_colour, 0, 0);

  current_tpos.x = tviewport.left;
  current_tpos.y = tviewport.top;
}
