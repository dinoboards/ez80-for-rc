#include "test_pattern.h"
#include "wait_for_key.h"
#include <ez80-firmware.h>
#include <stdio.h>
#include <v99x8.h>

void test_spike_pattern() {
  // clear the screen
  vdp_cmd_wait_completion();
  vdp_cmd_logical_move_vdp_to_vram(0, 0, get_screen_width(), get_screen_height(), 2, 0, 0);

  wait_for_key();

  // now make it a different colour
  vdp_cmd_wait_completion();
  vdp_cmd_logical_move_vdp_to_vram(0, 0, get_screen_width(), get_screen_height(), 3, 0, 0);

  // now draw vertical lines - gap by 10 from left to right

  for (uint24_t x = 0; x < get_screen_width(); x += 10) {
    // // Draw inner colored box
    vdp_cmd_wait_completion();

    vdp_cmd_logical_move_vdp_to_vram(x, 0, 1, get_screen_height(), 5, 0, DIX_RIGHT | DIY_DOWN);

    // vdp_cmd_line(x, 0, get_screen_height(), 1, 4, DIX_RIGHT | DIY_DOWN, CMD_LOGIC_IMP);
    printf("Line draw at %d\n", x);
    wait_for_key();
  }
}
void test_pattern(uint8_t col_row_count, uint8_t white_colour_index) {
  vdp_cmd_wait_completion();
  vdp_cmd_logical_move_vdp_to_vram(0, 0, get_screen_width(), get_screen_height(), white_colour_index, 0, 0);

  // Calculate box dimensions
  float box_width  = (float)get_screen_width() / (float)col_row_count;
  float box_height = (float)get_screen_height() / (float)col_row_count;
  float border     = 1.0; // Border thickness

  // grid of col_row_count
  uint8_t color = 0;

  for (uint8_t row = 0; row < col_row_count; row++) {
    for (uint8_t col = 0; col < col_row_count; col++) {
      float x = (float)col * box_width;
      float y = (float)row * box_height;

      // // Draw white border
      // vdp_cmd_wait_completion();
      // vdp_cmd_logical_move_vdp_to_vram(x, y, box_width, box_height, white_colour_index, 0, 0);

      // // Draw inner colored box
      vdp_cmd_wait_completion();
      vdp_cmd_logical_move_vdp_to_vram(x + border, y + border, box_width - (2.0 * border), box_height - (2.0 * border), color++, 0,
                                       0);

      // printf("colour box\n");
      // wait_for_key();
    }
  }

  for (int i = 0; i < 100; i++) {
    vdp_cmd_wait_completion();
    vdp_cmd_logical_move_vdp_to_vram(i + 10, i + 10, 100, 100, i, 0, 0);
    test_for_escape();
  }
  // printf("Lots of squares drawn\n");
  // wait_for_key();

  vdp_cmd_wait_completion();
  vdp_cmd_logical_move_vdp_to_vram(0, 0, 1, get_screen_height(), 3, 0, 0);
  test_for_escape();

  // printf("left line updated?\n");
  // wait_for_key();

  vdp_cmd_wait_completion();
  vdp_cmd_logical_move_vdp_to_vram(get_screen_width() - 1, 0, 1, get_screen_height(), 5, 0, 0);
  test_for_escape();

  // printf("right line updated?\n");
  // wait_for_key();

  vdp_cmd_wait_completion();
  vdp_cmd_move_vram_to_vram(0, 8, 0, 0, get_screen_width(), get_screen_height() - 8, DIX_RIGHT | DIY_DOWN);
  vdp_cmd_wait_completion();
  test_for_escape();

  // printf("Scroll up?\n");
  // wait_for_key();
  // do scroll again with vram_y

  // printf("do scroll again with vram_y\r\n");
  vdp_cmd_move_vram_to_vram_y(0, 8, 0, get_screen_height() - 8, DIX_RIGHT | DIY_DOWN);
  vdp_cmd_wait_completion();
  test_for_escape();

  // wait_for_key();

  // wait_for_key();
}
