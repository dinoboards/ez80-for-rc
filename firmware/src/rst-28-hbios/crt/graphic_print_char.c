#include "../rst-28-vars.h"
#include "v99x8-super.h"
#include "vdu/vdu_consts.h"
#include <stdint.h>

void graphic_print_char(uint24_t ch) {
  vdu_vars_t *const   vdu  = &hbios_vars->vdu;
  const screen_addr_t addr = FONT_8X8_STORED_Y + ch * 8;

  vdp_cmd_wait_completion();
  vdp_cmd_move_linear_to_xy(addr, vdu->current_tpos.x, vdu->current_tpos.y, 8, 8, DIX_RIGHT | DIY_DOWN, CMD_LOGIC_REMAP);

  vdu->current_tpos.x += 8;
  if (vdu->current_tpos.x > vdu->tviewport.right) {
    vdu_cr();
    vdu_lf();
  }
}
