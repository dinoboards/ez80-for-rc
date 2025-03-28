#include "../common/config_request.h"
#include "large_palette.h"
#include "wait_for_key.h"
#include <cpm.h>
#include <math.h>
#include <stdio.h>
#include <v99x8-super.h>

uint8_t source[100 * 100];

RGB default_palette[16] = {
    {0, 0, 0}, // Black
    {7, 0, 0}, // Bright Red
    {0, 7, 0}, // Bright Green
    {0, 0, 7}, // Bright Blue
    {7, 7, 7}, // White
    {4, 0, 0}, // Medium Red
    {0, 4, 0}, // Medium Green
    {0, 0, 4}, // Medium Blue
    {3, 3, 3}, // Gray
    {7, 7, 0}, // Yellow
    {7, 0, 7}, // Magenta
    {0, 7, 7}, // Cyan
    {5, 2, 0}, // Brown
    {2, 5, 2}, // Light Green
    {2, 2, 5}, // Light Blue
    {5, 5, 5}  // Light Gray
};
RGB palette_green[16] = {{0, 0, 0},   {0, 17, 0},  {0, 34, 0},  {0, 51, 0},  {0, 68, 0},  {0, 85, 0},  {0, 102, 0}, {0, 119, 0},
                         {0, 136, 0}, {0, 153, 0}, {0, 170, 0}, {0, 187, 0}, {0, 204, 0}, {0, 221, 0}, {0, 238, 0}, {0, 255, 0}};

RGB palette_blue[16] = {{0, 0, 0}, {0, 0, 1}, {0, 0, 2},  {0, 0, 3},  {0, 0, 4},  {0, 0, 5},  {0, 0, 6},  {0, 0, 7},
                        {0, 0, 8}, {0, 0, 9}, {0, 0, 10}, {0, 0, 11}, {0, 0, 12}, {0, 0, 13}, {0, 0, 14}, {0, 0, 15}};

RGB palette_red[16] = {{0, 0, 0}, {1, 0, 0}, {2, 0, 0},  {3, 0, 0},  {4, 0, 0},  {5, 0, 0},  {6, 0, 0},  {7, 0, 0},
                       {8, 0, 0}, {9, 0, 0}, {10, 0, 0}, {11, 0, 0}, {12, 0, 0}, {13, 0, 0}, {14, 0, 0}, {15, 0, 0}};

RGB palette_white[16] = {{0, 0, 0}, {1, 1, 1}, {2, 2, 2},    {3, 3, 3},    {4, 4, 4},    {5, 5, 5},    {6, 6, 6},    {7, 7, 7},
                         {8, 8, 8}, {9, 9, 9}, {10, 10, 10}, {11, 11, 11}, {12, 12, 12}, {13, 13, 13}, {14, 14, 14}, {15, 15, 15}};

RGB palette_contrast[16] = {{0, 0, 0},  {0, 15, 0},  {0, 0, 15},  {15, 15, 0}, {15, 0, 0}, {15, 0, 15}, {15, 7, 7}, {7, 15, 7},
                            {7, 7, 15}, {15, 15, 7}, {7, 15, 15}, {15, 7, 15}, {2, 2, 2},  {6, 6, 6},   {7, 7, 7},  {15, 15, 15}};

void vdp_clear_all_memory2(void) {
  DI;
  vdp_reg_write(14, 0);
  vdp_out_cmd(0);
  vdp_out_cmd(0x40);
  for (int i = 0; i < 720 * 576; i++)
    vdp_out_dat(0);
  EI;
}

uint24_t get_screen_width() { return vdp_get_screen_width(); }

uint24_t    get_screen_height() { return vdp_get_screen_height(); }
extern void graphics_mode_5_test_pattern(uint8_t refesh_rate);
extern void graphics_mode_6_test_pattern(uint8_t refesh_rate);
extern void super_graphics_mode_test_pattern(uint8_t gm);
extern void super_graphics_mode_double_buffering(uint8_t gm);
extern void graphics_mode_6_double_buffering();
extern void graphics_mode_7_double_buffering();

void main_double_buffering_test(void) {
  graphics_mode_6_double_buffering();

  vdp_set_super_graphic_2();
  super_graphics_mode_double_buffering(2);

  vdp_set_super_graphic_4();
  super_graphics_mode_double_buffering(4);

  vdp_set_super_graphic_6();
  super_graphics_mode_double_buffering(6);

  vdp_set_super_graphic_8();
  super_graphics_mode_double_buffering(8);

  vdp_set_super_graphic_1();
  super_graphics_mode_double_buffering(1);

  vdp_set_super_graphic_3();
  super_graphics_mode_double_buffering(3);

  vdp_set_super_graphic_5();
  super_graphics_mode_double_buffering(5);

  vdp_set_super_graphic_7();
  super_graphics_mode_double_buffering(7);
}

void main_patterns(void) {
  graphics_mode_5_test_pattern(50);
  graphics_mode_6_test_pattern(50);
  vdp_set_super_graphic_2();
  super_graphics_mode_test_pattern(2);

  vdp_set_super_graphic_4();
  super_graphics_mode_test_pattern(4);
  vdp_set_super_graphic_6();
  super_graphics_mode_test_pattern(6);
  vdp_set_super_graphic_8();
  super_graphics_mode_test_pattern(8);
  vdp_set_super_graphic_10();
  super_graphics_mode_test_pattern(10);
  vdp_set_super_graphic_12();
  super_graphics_mode_test_pattern(12);

  graphics_mode_5_test_pattern(60);
  graphics_mode_6_test_pattern(60);
  vdp_set_super_graphic_1();
  super_graphics_mode_test_pattern(1);
  vdp_set_super_graphic_3();
  super_graphics_mode_test_pattern(3);
  vdp_set_super_graphic_5();
  super_graphics_mode_test_pattern(5);
  vdp_set_super_graphic_7();
  super_graphics_mode_test_pattern(7);
  vdp_set_super_graphic_9();
  super_graphics_mode_test_pattern(9);
  vdp_set_super_graphic_11();
  super_graphics_mode_test_pattern(11);

  printf("Rotate through full set of green colours\r\n");
  wait_for_key();
  RGB green = {0, 0, 0};
  for (int i = 0; i < 256; i++) {
    green.green = i;
    vdp_set_extended_palette_entry(i, green);
  }

  printf("press key to exit\r\n");
  wait_for_key();
}

void main_test_vdp_cmd_logical_move_vram_to_vram() {
  // draw a rectangle in left, then copy it to the right, bottom/left and bottom/right
  vdp_set_super_graphic_1();

  // erase the screen
  vdp_cmd_wait_completion();
  vdp_cmd_logical_move_vdp_to_vram(0, 0, get_screen_width(), get_screen_height(), 15, 0, 0); // white

  uint24_t box_width  = get_screen_width() / 5;
  uint24_t box_height = get_screen_height() / 5;

  // draw box in the top/left
  vdp_cmd_wait_completion();
  vdp_cmd_logical_move_vdp_to_vram(10, 10, box_width, box_height, 2, 0, 0);

  // copy it to the right
  vdp_cmd_wait_completion();
  vdp_cmd_logical_move_vram_to_vram(10, 10, get_screen_width() / 2 + 10, 10, box_width, box_height, DIX_RIGHT | DIY_DOWN,
                                    CMD_LOGIC_IMP);

  // copy it to the right/bottom
  vdp_cmd_wait_completion();
  vdp_cmd_logical_move_vram_to_vram(10, 10, get_screen_width() / 2 + 10, get_screen_height() / 2 + 10, box_width, box_height,
                                    DIX_RIGHT | DIY_DOWN, CMD_LOGIC_IMP);

  // copy it to the left/bottom
  vdp_cmd_wait_completion();
  vdp_cmd_logical_move_vram_to_vram(10, 10, 10, get_screen_height() / 2 + 10, box_width, box_height, DIX_RIGHT | DIY_DOWN,
                                    CMD_LOGIC_IMP);

  wait_for_key();
}

void main_test_vdp_cmd_move_vram_to_vram() {
  // draw a rectangle in left, then copy it to the right, bottom/left and bottom/right
  vdp_set_super_graphic_1();

  // erase the screen
  vdp_cmd_wait_completion();
  vdp_cmd_logical_move_vdp_to_vram(0, 0, get_screen_width(), get_screen_height(), 15, 0, 0); // white

  uint24_t box_width  = get_screen_width() / 5;
  uint24_t box_height = get_screen_height() / 5;

  // draw box in the top/left
  vdp_cmd_wait_completion();
  vdp_cmd_logical_move_vdp_to_vram(10, 10, box_width, box_height, 2, 0, 0);

  // copy it to the right
  vdp_cmd_wait_completion();
  vdp_cmd_move_vram_to_vram(10, 10, get_screen_width() / 2 + 10, 10, box_width, box_height, DIX_RIGHT | DIY_DOWN);

  // copy it to the right/bottom
  vdp_cmd_wait_completion();
  vdp_cmd_move_vram_to_vram(10, 10, get_screen_width() / 2 + 10, get_screen_height() / 2 + 10, box_width, box_height,
                            DIX_RIGHT | DIY_DOWN);

  // copy it to the left/bottom
  vdp_cmd_wait_completion();
  vdp_cmd_move_vram_to_vram(10, 10, 10, get_screen_height() / 2 + 10, box_width, box_height, DIX_RIGHT | DIY_DOWN);

  wait_for_key();
}

void main_vram_test() {
  // try to erase video memory

  vdp_set_super_graphic_10();

  vdp_set_extended_palette(large_palette);

  vdp_reg_write(7, 4);

  uint8_t data = 1;
  printf("Press key to 'erase''\r\n");
  wait_for_key();

  for (uint24_t i = 0; i < get_screen_width() * 10; i++) {
    vdp_cpu_to_vram(&data, i, 1);
    test_for_escape();
    vdp_cpu_to_vram(&data, i, 1);
  }
  printf("erased?\r\n");
  wait_for_key();

  data = 2;
  for (uint24_t i = 0; i < get_screen_width() * 10; i++) {
    vdp_cpu_to_vram(&data, i, 1);
    printf("Byte: %d\r\n", i);
  }

  printf("confirm\r\n");
  wait_for_key();
}

#define VDP_TMS   1
#define VDP_V9938 2
#define VDP_V9958 3
#define VDP_SUPER 4

int main() {
  uint8_t r = vdp_init();
  switch (r) {
  case VDP_TMS:
    printf("VDP Detected: TMS\r\n");
    break;

  case VDP_V9938:
    printf("VDP Detected: V9938\r\n");
    break;

  case VDP_V9958:
    printf("VDP Detected: V9958\r\n");
    break;

  case VDP_SUPER:
    printf("VDP Detected: SUPER HDMI\r\n");
    break;
  }
  // main_vram_test();

  // main_test_vdp_cmd_move_vram_to_vram();

  // main_test_vdp_cmd_logical_move_vram_to_vram();

  // main_double_buffering_test();

  main_patterns();

  return 0;
}
