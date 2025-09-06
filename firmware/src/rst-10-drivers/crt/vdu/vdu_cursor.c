#include "../vdu.h"
#include <eZ80F92-extra.h>
#include <stdint.h>
#include <v99x8-super.h>
#include <v99x8.h>
#include <vm-shared-mem.h>

screen_addr_t cursor_pattern_addr = FONT_8X8_STORED_Y - (8);

void vdu_cursor_disable() {
  di_and_save();
  cursor_state.enabled = 0;

  if (cursor_state.toggle) {
    vdp_cmd_wait_completion();
    vdp_cmd_move_linear_to_xy(cursor_pattern_addr, current_tpos.x, current_tpos.y, 8, 8, DIX_RIGHT | DIY_DOWN, CMD_LOGIC_REMAP_XOR);
    cursor_state.toggle = 0;
  }
  restore_ei();
}

void vdu_cursor_enable() {
  di_and_save();
  cursor_state.enabled = 1;

  if (cursor_state.toggle) {
    vdp_cmd_wait_completion();
    vdp_cmd_move_linear_to_xy(cursor_pattern_addr, current_tpos.x, current_tpos.y, 8, 8, DIX_RIGHT | DIY_DOWN, CMD_LOGIC_REMAP_XOR);
    cursor_state.toggle = 0;
  }

  restore_ei();
}

// NOTE: this function called via timer interrupts handler

void vdu_toggle_cursor() {
  if (!cursor_state.enabled)
    return;

  cursor_state.toggle = ~cursor_state.toggle;

  vdp_cmd_wait_completion();
  vdp_cmd_move_linear_to_xy(cursor_pattern_addr, current_tpos.x, current_tpos.y, 8, 8, DIX_RIGHT | DIY_DOWN, CMD_LOGIC_REMAP_XOR);
  vdp_cmd_wait_completion();
}
