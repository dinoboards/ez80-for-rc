#include "test_pattern.h"
#include "wait_for_key.h"
#include <stdio.h>
#include <v99x8.h>

static RGB palette[16] = {
    {0, 0, 0},       // Black
    {255, 0, 0},     // Bright Red
    {0, 255, 0},     // Bright Green
    {255, 255, 255}, // White
};

/*
 * Graphics Mode 5 characteristics:
 * - Resolution: 512 x 212 pixels (50Hz) or 512 x 192 pixels (60Hz)
 * - Colors: 4 colors per screen from a palette of 512 colors
 * - VRAM Usage: 32KB per screen

* Generate a test pattern - a series of rectangles showing all 16 colours
* populate the palette of 16, with a VGA like standard colour palette

*/
void graphics_mode_5_test_pattern(uint8_t refesh_rate) {
  vdp_set_refresh(refesh_rate);
  for (int l = 192; l <= 212; l += 20) {
    vdp_set_lines(l);
    vdp_set_graphic_5();
    vdp_set_palette(palette);

    printf("Graphics Mode 5 (%d x %d) @ %dHz, 4 Colours\r\n", vdp_get_screen_width(), vdp_get_screen_height(), refesh_rate);

    test_pattern(2, 1);

    // wait_for_key();
  }
}
