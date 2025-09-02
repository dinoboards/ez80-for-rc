#include "../vm-promoted-fn.h"
#include "vdu/variables.h"
#include <host-functions.h>
#include <stdint.h>
#include <v99x8-super.h>

void graphic_print_char(uint24_t ch) {
  vdp_set_remap(current_tbg_colour, current_tfg_colour); /* TODO: move this to where we set text colours */

  {
    screen_addr_t addr = FONT_8X8_STORED_Y + (ch * 8);

    vdp_cmd_wait_completion();
    vdp_cmd_move_linear_to_xy(addr, current_tpos.x, current_tpos.y, 8, 8, DIX_RIGHT | DIY_DOWN, CMD_LOGIC_REMAP);

    current_tpos.x += 8;
    if (current_tpos.x > tviewport.right) {
      vdu_cr();
      vdu_lf();
    }
  }
}
