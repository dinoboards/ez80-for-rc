#include "test_pattern.h"
#include "wait_for_key.h"
#include <stdio.h>
#include <v99x8.h>

static RGB palette[16] = {
    {0, 0, 0},       // Black
    {255, 0, 0},     // Bright Red
    {0, 255, 0},     // Bright Green
    {0, 0, 255},     // Bright Blue
    {255, 255, 255}, // White
    {146, 0, 0},     // Medium Red
    {0, 146, 0},     // Medium Green
    {0, 0, 146},     // Medium Blue
    {109, 109, 109}, // Gray
    {255, 255, 0},   // Yellow
    {255, 0, 255},   // Magenta
    {0, 255, 255},   // Cyan
    {182, 73, 0},    // Brown
    {73, 182, 73},   // Light Green
    {73, 73, 182},   // Light Blue
    {182, 182, 182}  // Light Gray
};

/*
* Graphics Mode 6 characteristics:
* - Resolution: 512 x 212 pixels (50Hz) or 512 x 192 pixels (60Hz)
* - Colors: 16 colors per screen from a palette of 512 colors

* Generate a test pattern - a series of rectangles showing all 16 colours
* populate the palette of 16, with a VGA like standard colour palette

*/
void graphics_mode_6_test_pattern(uint8_t refesh_rate) {
  vdp_set_refresh(refesh_rate);
  for (int l = 192; l <= 212; l += 20) {
    vdp_set_lines(l);
    vdp_set_graphic_6();
    vdp_set_palette(palette);

    printf("Graphics Mode 6 (%d x %d) @ %dHz, 16 Colours\r\n", vdp_get_screen_width(), vdp_get_screen_height(), refesh_rate);

    test_pattern(4, 1);
    // wait_for_key();
  }
}

void graphics_mode_6_double_buffering() {
  vdp_set_refresh(50);
  vdp_set_lines(212);
  vdp_set_graphic_6();
  // vdp_set_palette(palette);

  printf("Graphics Mode 7 (%d x %d), 256 Colours, Double Buffer Test\r\n", get_screen_width(), get_screen_height());

  vdp_cmd_logical_move_vdp_to_vram(0, 0, get_screen_width(), get_screen_height(), 1, 0, 0);
  vdp_cmd_wait_completion();

  // clear page 2 with green
  vdp_cmd_logical_move_vdp_to_vram(0, 256, get_screen_width(), get_screen_height(), 2, 0, 0);
  vdp_cmd_wait_completion();

  // draw a box in the middle of the 2nd screen
  const uint16_t box_width  = 100;
  const uint16_t box_height = 100;

  const uint16_t box_x = (get_screen_width() - box_width) / 2;
  const uint16_t box_y = (get_screen_height() - box_height) / 2;

  vdp_cmd_logical_move_vdp_to_vram(box_x, box_y + 256, box_width, box_height, 3, 0, 0);
  vdp_cmd_wait_completion();

  printf("Press key to flip to 2nd page\r\n");
  wait_for_key();

  vdp_set_page(1);
  // vdp_set_command_page(0);

  printf("Press key to flip back to 1st page\r\n");
  wait_for_key();
  vdp_cmd_logical_move_vdp_to_vram(0, 0, box_width, box_height, 4, 0, 0); // box in top left corner
  vdp_cmd_wait_completion();

  vdp_set_page(0);

  printf("Press key to continue\r\n");
  wait_for_key();
}
