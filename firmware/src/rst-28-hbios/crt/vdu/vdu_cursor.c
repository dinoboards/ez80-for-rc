#include "../rst-28-vars.h"
#include "../vdu.h"
#include "vdu_consts.h"
#include <eZ80F92-extra.h>
#include <stdint.h>
#include <v99x8-super.h>
#include <v99x8.h>

static const screen_addr_t cursor_pattern_addr = FONT_8X8_STORED_Y - (8);

void vdu_cursor_disable(void) {
  vdu_vars_t *const vdu = &hbios_vars->vdu;

  di_and_save();
  vdu->cursor_state.enabled = 0;

  if (vdu->cursor_state.toggle) {
    vdp_cmd_wait_completion();
    vdp_cmd_move_linear_to_xy(cursor_pattern_addr, vdu->current_tpos.x, vdu->current_tpos.y, 8, 8, DIX_RIGHT | DIY_DOWN,
                              CMD_LOGIC_REMAP_XOR);
    vdu->cursor_state.toggle = 0;
  }
  restore_ei();
}

void vdu_cursor_enable(void) {
  vdu_vars_t *const vdu = &hbios_vars->vdu;

  di_and_save();
  vdu->cursor_state.enabled = 1;

  if (vdu->cursor_state.toggle) {
    vdp_cmd_wait_completion();
    vdp_cmd_move_linear_to_xy(cursor_pattern_addr, vdu->current_tpos.x, vdu->current_tpos.y, 8, 8, DIX_RIGHT | DIY_DOWN,
                              CMD_LOGIC_REMAP_XOR);
    vdu->cursor_state.toggle = 0;
  }

  restore_ei();
}

// NOTE: this function called via timer interrupts handler

void vdu_toggle_cursor(void) {
  vdu_vars_t *const vdu = &hbios_vars->vdu;

  if (!vdu->cursor_state.enabled)
    return;

  vdu->cursor_state.toggle = ~vdu->cursor_state.toggle;

  vdp_cmd_wait_completion();
  vdp_cmd_move_linear_to_xy(cursor_pattern_addr, vdu->current_tpos.x, vdu->current_tpos.y, 8, 8, DIX_RIGHT | DIY_DOWN,
                            CMD_LOGIC_REMAP_XOR);
  vdp_cmd_wait_completion();
}
