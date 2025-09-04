#include "v99x8-super.h"
#include "vdu/variables.h"
#include <stdint.h>

void graphic_print_char(uint24_t ch) {

  screen_addr_t addr = FONT_8X8_STORED_Y + ch * 8;
  vdp_cmd_wait_completion();
  vdp_cmd_move_linear_to_xy(addr, sm.current_tpos.x, sm.current_tpos.y, 8, 8, DIX_RIGHT | DIY_DOWN, CMD_LOGIC_REMAP);

  sm.current_tpos.x += 8;
  if (sm.current_tpos.x > sm.tviewport.right) {
    vdu_cr();
    vdu_lf();
  }
}
