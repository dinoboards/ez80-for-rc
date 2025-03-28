#include "large_palette.h"
#include "test_pattern.h"
#include "wait_for_key.h"
#include <stdio.h>
#include <v99x8-super.h>

void graphics_mode_7_double_buffering() {
  vdp_set_refresh(50);
  vdp_set_lines(212);
  vdp_set_graphic_7();
  // vdp_set_palette(palette);

  printf("Graphics Mode 7 (%d x %d), 256 Colours, Double Buffer Test\r\n", get_screen_width(), get_screen_height());

  vdp_cmd_logical_move_vdp_to_vram(0, 0, get_screen_width(), get_screen_height(), 8, 0, 0);
  vdp_cmd_wait_completion();

  // clear page 2 with green
  vdp_cmd_logical_move_vdp_to_vram(0, 256, get_screen_width(), get_screen_height(), 0, 0, 0);
  vdp_cmd_wait_completion();

  // draw a box in the middle of the 2nd screen
  const uint16_t box_width  = 100;
  const uint16_t box_height = 100;

  const uint16_t box_x = (get_screen_width() - box_width) / 2;
  const uint16_t box_y = (get_screen_height() - box_height) / 2;

  vdp_cmd_logical_move_vdp_to_vram(box_x, box_y + 256, box_width, box_height, 128, 0, 0);
  vdp_cmd_wait_completion();

  printf("Press key to flip to 2nd page\r\n");
  wait_for_key();

  vdp_set_page(1);
  // vdp_set_command_page(0);

  printf("Press key to flip back to 1st page\r\n");
  wait_for_key();
  vdp_cmd_logical_move_vdp_to_vram(0, 0, box_width, box_height, 32, 0, 0); // box in top left corner
  vdp_cmd_wait_completion();

  vdp_set_page(0);

  printf("Press key to continue\r\n");
  wait_for_key();
}
