#include "large_palette.h"
#include "test_pattern.h"
#include "wait_for_key.h"
#include <stdio.h>
#include <v99x8-super.h>

extern void log_mode();

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

void graphics_mode_7_double_buffering() {
  vdp_set_refresh(50);
  vdp_set_lines(212);
  vdp_set_graphic_7();
  // vdp_set_palette(palette);

  log_mode();
  printf(" Double Buffer Test\n");

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

uint8_t buffer[50 * 50];

void graphics_mode_7_logical_transforms() {
  vdp_set_refresh(50);
  vdp_set_lines(212);
  vdp_set_graphic_7();
  vdp_set_palette(palette);

  log_mode();
  printf("Logical Transform\n");

  // erase screen with cpu to vram operation
  vdp_cmd_move_data_to_vram(0, 0, 0, 256, 212, DIX_RIGHT | DIY_DOWN, 256 * 212);
  for (int i = 1; i < 256 * 212; i++) {
    vdp_cmd_send_byte(i);
    // test_for_escape();
  }

  printf("Press key to apply logical transform operation\r\n");
  wait_for_key();

  for (int i = 0; i < 50 * 50; i++) {
    if (i % 4 == 0)
      buffer[i] = 65;
    else
      buffer[i] = 0;
  }

  vdp_cmd_logical_move_cpu_to_vram(buffer, 100, 100, 50, 50, DIX_RIGHT | DIY_DOWN, 50 * 50, CMD_LOGIC_TIMP);

  vdp_cmd_logical_move_data_to_vram(0, 180, 100, 50, 50, DIX_RIGHT | DIY_DOWN, 50 * 50, CMD_LOGIC_TIMP);
  for (int i = 1; i < 50 * 50; i++) {
    if (i % 4 != 0)
      vdp_cmd_send_byte(255);
    else
      vdp_cmd_send_byte(0);
  }

  printf("Press any key to continue\r\n");
  wait_for_key();
}
