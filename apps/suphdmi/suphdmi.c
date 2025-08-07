#include "../common/config_request.h"
#include "palettes.h"
#include "wait_for_key.h"
#include <cpm.h>
#include <ez80-firmware.h>
#include <math.h>
#include <stdio.h>
#include <v99x8-super.h>

void sleep_a_bit() {
  for (volatile int i = 0; i < 700; i++)
    ;
}

void vdp_clear_all_memory2(void) {
  DI();
  vdp_reg_write(14, 0);
  vdp_out_cmd(0);
  vdp_out_cmd(0x40);
  for (int i = 0; i < 720 * 576; i++)
    vdp_out_dat(0);
  EI();
}

uint8_t super_graphic_50hz_modes[] = {0x02, 0x04, 0x06, 0x08, 0x09, 0x0A, 0x0C, 0x16, 0x18, 0x19, 0x1A, 0x1C};

uint8_t super_graphic_60hz_modes[] = {0x01, 0x03, 0x05, 0x07, 0x0B, 0x15, 0x17, 0x1B};

extern void graphics_mode_test_pattern(uint8_t mode, uint8_t refresh_rate, RGB *palette);
extern void super_graphics_mode_test_pattern();
extern void super_graphics_mode_double_buffering();
extern void graphics_mode_6_double_buffering();
extern void graphics_mode_7_double_buffering();

void log_mode() {
  uint8_t mode = vdp_get_graphic_mode();
  if (mode >= 0x80)
    printf("Super Graphics Mode %d (%d x %d), %d Colours, @ %dHz\n", mode & 0x7f, vdp_get_screen_width(), vdp_get_screen_height(),
           vdp_get_screen_max_unique_colours(), vdp_get_refresh());
  else
    printf("Graphics Mode %d (%d x %d), %d Colours, @ %dHz\n", mode, vdp_get_screen_width(), vdp_get_screen_height(),
           vdp_get_screen_max_unique_colours(), vdp_get_refresh());
}

uint8_t get_pixel_per_byte() {
  const uint24_t r = vdp_get_screen_max_unique_colours();
  switch (r) {
  case 256:
    return 1;

  case 16:
    return 2;

  case 4:
    return 4;

  default:
    return 1;
  }
}

void main_double_buffering_test(void) {
  graphics_mode_6_double_buffering();

#ifdef VDP_SUPER_HDMI
  vdp_set_super_graphic_2();
  super_graphics_mode_double_buffering();

  vdp_set_super_graphic_4();
  super_graphics_mode_double_buffering();

  vdp_set_super_graphic_6();
  super_graphics_mode_double_buffering();

  vdp_set_super_graphic_8();
  super_graphics_mode_double_buffering();

  vdp_set_super_graphic_1();
  super_graphics_mode_double_buffering();

  vdp_set_super_graphic_3();
  super_graphics_mode_double_buffering();

  vdp_set_super_graphic_5();
  super_graphics_mode_double_buffering();

  vdp_set_super_graphic_7();
  super_graphics_mode_double_buffering();
#endif
}

#ifdef VDP_SUPER_HDMI
void rotate_256_palettes() {
  printf("Set palette to range of 256 green colours\r\n");
  // wait_for_key();
  RGB green = {0, 0, 0};
  for (int i = 0; i < 256; i++) {
    green.green = i;
    vdp_set_extended_palette_entry(i, green);
  }
  // wait_for_key();
}

void rotate_16_palettes() {
  printf("Set palette to range of 16 green colours\r\n");
  // wait_for_key();
  RGB green = {0, 0, 0};
  for (int i = 0; i < 16; i++) {
    green.green = i * 16;
    vdp_set_extended_palette_entry(i, green);
  }
  // wait_for_key();
}
#endif

void main_patterns(void) {
  graphics_mode_test_pattern(4, 60, palette_16);
  graphics_mode_test_pattern(5, 60, palette_4);

#ifdef VDP_SUPER_HDMI
  while (true) {
    for (uint8_t m = 0; m < sizeof(super_graphic_60hz_modes); m++) {
      vdp_set_super_graphic_mode(super_graphic_60hz_modes[m]);
      super_graphics_mode_test_pattern();

      if (get_pixel_per_byte() == 2)
        rotate_16_palettes();
      else
        rotate_256_palettes();

      sleep_ms(1000);
    }

    for (uint8_t m = 0; m < sizeof(super_graphic_50hz_modes); m++) {
      vdp_set_super_graphic_mode(super_graphic_50hz_modes[m]);
      super_graphics_mode_test_pattern();

      if (get_pixel_per_byte() == 2)
        rotate_16_palettes();
      else
        rotate_256_palettes();

      sleep_ms(1000);
    }
  }
#endif
}

#ifdef VDP_SUPER_HDMI
void main_test_vdp_cmd_logical_move_vram_to_vram() {
  // draw a rectangle in left, then copy it to the right, bottom/left and bottom/right
  vdp_set_super_graphic_1();

  // erase the screen
  vdp_cmd_wait_completion();
  vdp_cmd_logical_move_vdp_to_vram(0, 0, vdp_get_screen_width(), vdp_get_screen_height(), 15, 0, 0); // white

  uint24_t box_width  = vdp_get_screen_width() / 5;
  uint24_t box_height = vdp_get_screen_height() / 5;

  // draw box in the top/left
  vdp_cmd_wait_completion();
  vdp_cmd_logical_move_vdp_to_vram(10, 10, box_width, box_height, 2, 0, 0);

  // copy it to the right
  vdp_cmd_wait_completion();
  vdp_cmd_logical_move_vram_to_vram(10, 10, vdp_get_screen_width() / 2 + 10, 10, box_width, box_height, DIX_RIGHT | DIY_DOWN,
                                    CMD_LOGIC_IMP);

  // copy it to the right/bottom
  vdp_cmd_wait_completion();
  vdp_cmd_logical_move_vram_to_vram(10, 10, vdp_get_screen_width() / 2 + 10, vdp_get_screen_height() / 2 + 10, box_width,
                                    box_height, DIX_RIGHT | DIY_DOWN, CMD_LOGIC_IMP);

  // copy it to the left/bottom
  vdp_cmd_wait_completion();
  vdp_cmd_logical_move_vram_to_vram(10, 10, 10, vdp_get_screen_height() / 2 + 10, box_width, box_height, DIX_RIGHT | DIY_DOWN,
                                    CMD_LOGIC_IMP);
}
#endif

#ifdef VDP_SUPER_HDMI
void main_test_vdp_cmd_move_vram_to_vram() {
  // draw a rectangle in left, then copy it to the right, bottom/left and bottom/right
  vdp_set_super_graphic_1();

  // erase the screen
  vdp_cmd_wait_completion();
  vdp_cmd_logical_move_vdp_to_vram(0, 0, vdp_get_screen_width(), vdp_get_screen_height(), 15, 0, 0); // white

  uint24_t box_width  = vdp_get_screen_width() / 5;
  uint24_t box_height = vdp_get_screen_height() / 5;

  // draw box in the top/left
  vdp_cmd_wait_completion();
  vdp_cmd_logical_move_vdp_to_vram(10, 10, box_width, box_height, 2, 0, 0);

  // copy it to the right
  vdp_cmd_wait_completion();
  vdp_cmd_move_vram_to_vram(10, 10, vdp_get_screen_width() / 2 + 10, 10, box_width, box_height, DIX_RIGHT | DIY_DOWN);

  // copy it to the right/bottom
  vdp_cmd_wait_completion();
  vdp_cmd_move_vram_to_vram(10, 10, vdp_get_screen_width() / 2 + 10, vdp_get_screen_height() / 2 + 10, box_width, box_height,
                            DIX_RIGHT | DIY_DOWN);

  // copy it to the left/bottom
  vdp_cmd_wait_completion();
  vdp_cmd_move_vram_to_vram(10, 10, 10, vdp_get_screen_height() / 2 + 10, box_width, box_height, DIX_RIGHT | DIY_DOWN);
}
#endif

void fill(const uint8_t fill_data, const uint24_t count, uint24_t from, const uint24_t increment) {
  for (uint24_t i = 0; i < count; i++) {
    vdp_cpu_to_vram(&fill_data, from, 1);
    sleep_a_bit();
    from += increment;
    test_for_escape();
  }
}

#ifdef VDP_SUPER_HDMI

#define RGB_256_GREEN  2
#define RGB_256_YELLOW 3
#define RGB_256_PURPLE 5
#define RGB_256_CYAN   6
#define RGB_256_RED    9
#define RGB_256_WHITE  8

void main_vram_test() {
  vdp_set_extended_palette(palette_256);
  log_mode();

  uint8_t       data = 0;
  const uint8_t ppb  = get_pixel_per_byte();

  const uint24_t byte_width = vdp_get_screen_width() / ppb;

  vdp_cmd_logical_move_vdp_to_vram(0, 0, vdp_get_screen_width(), vdp_get_screen_height() + 10, 4, 0, 0);
  vdp_cmd_wait_completion();

  vdp_cmd_logical_move_vdp_to_vram(0, 0, vdp_get_screen_width(), vdp_get_screen_height(), 0, 0, 0);
  vdp_cmd_wait_completion();

  data = RGB_256_GREEN;
  vdp_cpu_to_vram(&data, byte_width * vdp_get_screen_height() - 1, 1);
  sleep_a_bit();

  // top row - alt colours
  fill(ppb == 2 ? RGB_256_YELLOW | (RGB_256_YELLOW << 4) : RGB_256_YELLOW, byte_width, 0, 1);

  // 2nd row all one colour
  fill(ppb == 2 ? 5 | (5 << 4) : 5, byte_width, byte_width, 1);

  // fill first column
  fill(ppb == 2 ? RGB_256_YELLOW << 4 : RGB_256_YELLOW, vdp_get_screen_height(), 0, byte_width);

  // fill second column
  fill(ppb == 2 ? RGB_256_YELLOW << 4 | RGB_256_GREEN : RGB_256_GREEN, vdp_get_screen_height(), ppb == 2 ? 0 : 1, byte_width);

  // fill third column
  fill(ppb == 2 ? RGB_256_CYAN << 4 : RGB_256_CYAN, vdp_get_screen_height(), ppb == 2 ? 1 : 2, byte_width);

  // fill fourth column
  fill(ppb == 2 ? RGB_256_CYAN << 4 | RGB_256_WHITE : RGB_256_WHITE, vdp_get_screen_height(), ppb == 2 ? 1 : 3, byte_width);

  // fill fifth column
  fill(ppb == 2 ? RGB_256_RED << 4 : RGB_256_RED, vdp_get_screen_height(), ppb == 2 ? 2 : 4, byte_width);

  data = RGB_256_WHITE;
  for (uint8_t phase = 0; phase < 2; phase++) {
    uint24_t b = byte_width / 2;
    if (ppb == 2) {
      for (uint24_t x = 0; x <= 5; x++)
        fill(data, vdp_get_screen_height(), b + x, byte_width);
      data = RGB_256_WHITE << 4;
    } else {
      for (uint24_t x = 0; x <= 5; x++)
        fill(data, vdp_get_screen_height(), b + x * 2 + phase, byte_width);
    }
  }

  // fill right most column
  fill(RGB_256_YELLOW, vdp_get_screen_height(), byte_width - 1, byte_width);

  // bottom row
  fill(ppb == 2 ? RGB_256_YELLOW << 4 | RGB_256_YELLOW : RGB_256_YELLOW, vdp_get_screen_width(),
       byte_width * (vdp_get_screen_height() - 1), 1);
}
#endif

#ifdef VDP_SUPER_HDMI

void main_vram_tests(void) {
  for (uint8_t m = 0; m < sizeof(super_graphic_60hz_modes); m++) {
    vdp_set_super_graphic_mode(super_graphic_60hz_modes[m]);
    main_vram_test();
    sleep_ms(1000);
  }

  for (uint8_t m = 0; m < sizeof(super_graphic_50hz_modes); m++) {
    vdp_set_super_graphic_mode(super_graphic_50hz_modes[m]);
    main_vram_test();
    sleep_ms(1000);
  }
}
#endif

#define VDP_TMS   1
#define VDP_V9938 2
#define VDP_V9958 3
#define VDP_SUPER 4

uint8_t source[128 * 128];

#ifdef VDP_SUPER_HDMI
void main_test_transfers() {

  vdp_set_super_graphic_1();
  vdp_set_extended_palette(palette_256);
  vdp_reg_write(7, 4);

  vdp_cmd_logical_move_vdp_to_vram(0, 0, vdp_get_screen_width(), vdp_get_screen_height(), 0, 0, 0);
  vdp_cmd_wait_completion();

  for (int i = 0; i < 128 * 128; i++)
    source[i] = 3;

  printf("Press key to render square\r\n");
  wait_for_key();

  vdp_cmd_move_cpu_to_vram(source, 50, 50, 128, 128, DIX_RIGHT | DIY_DOWN, 128 * 128);
  // vdp_cmd_move_data_to_vram(source[0], 0, 0, 4, 5, DIX_RIGHT | DIY_DOWN, 4*5);

  // for(int i = 1; i< 4*5; i++) {
  //   vdp_cmd_send_byte(source[i]);
  //   uint8_t r = vdp_get_status(2) & 0x80;
  //   if (!r)
  //     printf("i=%d, r=%d\r\n", i, r);
  // }

  printf("Press key to exit\r\n");
  wait_for_key();
}
#endif

extern void graphics_mode_7_logical_transforms();

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

#ifdef VDP_SUPER_HDMI
  // main_vram_tests();
#endif
  // main_test_vdp_cmd_move_vram_to_vram();

  // main_test_vdp_cmd_logical_move_vram_to_vram();

  // main_double_buffering_test();

  main_patterns();

  // while(true) {
  //   vdp_set_super_graphic_3();
  //   super_graphics_mode_test_pattern(3);
  // }
  // main_test_transfers();

  // graphics_mode_7_logical_transforms();

  return 0;
}
