#include "large_palette.h"
#include "test_pattern.h"
#include "wait_for_key.h"
#include <stdio.h>
#include <v99x8-super.h>

/*
* Generate a test pattern - a series of rectangles showing all 256 colours
* populate the palette of 256, with a VGA like standard colour palette

*/

#ifdef VDP_SUPER_HDMI
void super_graphics_mode_test_pattern(uint8_t gm) {
  vdp_set_extended_palette(large_palette);

  printf("Super Graphics Mode %d (%d x %d), 256 Colours\r\n", gm, get_screen_width(), get_screen_height());

  test_pattern(16, 15);
  // wait_for_key();
}

void super_graphics_mode_double_buffering(uint8_t gm) {
  vdp_set_extended_palette(large_palette);

  printf("Super Graphics Mode %d (%d x %d), 256 Colours, Double Buffer Test\r\n", gm, get_screen_width(), get_screen_height());

  vdp_cmd_logical_move_vdp_to_vram(0, 0, get_screen_width(), get_screen_height(), 1, 0, 0);
  vdp_cmd_wait_completion();

  vdp_set_command_page(1);
  vdp_cmd_logical_move_vdp_to_vram(0, 0, get_screen_width(), get_screen_height(), 2, 0, 0);
  vdp_cmd_wait_completion();

  // draw a box in the middle of the 2nd screen
  const uint16_t box_width  = 100;
  const uint16_t box_height = 100;

  const uint16_t box_x = (get_screen_width() - box_width) / 2;
  const uint16_t box_y = (get_screen_height() - box_height) / 2;

  vdp_cmd_logical_move_vdp_to_vram(box_x, box_y, box_width, box_height, 3, 0, 0);
  vdp_cmd_wait_completion();

  printf("Press key to flip to 2nd page\r\n");
  wait_for_key();

  vdp_set_page(1);
  vdp_set_command_page(0);

  printf("Press key to flip back to 1st page\r\n");
  wait_for_key();
  vdp_cmd_logical_move_vdp_to_vram(0, 0, box_width, box_height, 4, 0, 0);
  vdp_cmd_wait_completion();

  vdp_set_page(0);

  printf("Press key to continue\r\n");
  wait_for_key();
}
#endif
