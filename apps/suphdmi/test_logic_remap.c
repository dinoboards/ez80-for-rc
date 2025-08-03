#include "palettes.h"
#include "wait_for_key.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <v99x8-super.h>

extern void prepare_font_pattern(uint8_t ch, uint16_t gpos_x, uint16_t gpos_y);

#define VRAM_SIZE 1048575

#define FONT_COUNT                 256
#define FONT_8X8_PER_ROW           32
#define FONT_8X8_TOTAL_STORED_ROWS (FONT_COUNT / 32) /* 8 rows */

// find the total number of lines available assuming a screen width of 720
#define FONT_8X8_MAX_SCREEN_LINES (VRAM_SIZE / 720)
#define FONT_8X8_MAX_SCREEN_ROWS  (FONT_8X8_MAX_SCREEN_LINES / 8)

// Find the highest Y position to store
// fonts at 32 per row
// for 8 rows
// for any screen resolution
#define FONT_8X8_STORED_Y ((uint16_t)(FONT_8X8_MAX_SCREEN_ROWS - FONT_8X8_TOTAL_STORED_ROWS) * 8)

#define crt_font_8x8_stored_x(ch) ((ch % FONT_8X8_PER_ROW) * 8)
#define crt_font_8x8_stored_y(ch) (FONT_8X8_STORED_Y + (ch / FONT_8X8_PER_ROW) * 8)

extern const uint8_t sysfont[(128 - ' ') * 8]; // 96*8

void crt_font_8x8_load_patterns(const uint8_t *const font_patterns, const uint8_t start_ch, const uint8_t number_of_ch) {
  uint24_t       gpos_x = crt_font_8x8_stored_x(start_ch);
  uint24_t       gpos_y = crt_font_8x8_stored_y(start_ch);
  const uint8_t *p      = &font_patterns[0];

  // DI();

  vdp_reg_write(45, 0);

  const uint24_t length = number_of_ch == 0 ? 256 : (uint24_t)number_of_ch;
  printf("Loading from %d for length of %d\n", start_ch, length);
  for (uint24_t ch = start_ch; ch < length; ch++) {
    for (int y = 0; y < 8; y++) {
      const uint8_t r = *p++;

      vdp_reg_write(38, gpos_y & 0xFF);
      vdp_reg_write(39, (gpos_y >> 8) & 255);

      for (int x = 0; x < 8; x++) {
        const bool pixel_on = (r & (1 << (7 - x)));

        vdp_reg_write(36, gpos_x & 0xFF);
        vdp_reg_write(37, (gpos_x >> 8) & 255);

        if (pixel_on) {
          vdp_reg_write(44, 1); // color
        } else
          vdp_reg_write(44, 0); // color

        vdp_cmd_wait_completion();
        vdp_reg_write(46, CMD_PSET | CMD_LOGIC_IMP);

        gpos_x++;
      }

      gpos_x -= 8;
      gpos_y++;
    }

    if (gpos_x >= (FONT_8X8_PER_ROW - 1) * 8) {
      gpos_x = 0;
    } else {
      gpos_x += 8;
      gpos_y -= 8;
    }
  }

  // EI();
}

void crt_font_8x8_clear_patterns() {
  vdp_cmd_wait_completion();
  vdp_cmd_logical_move_vdp_to_vram(0, FONT_8X8_STORED_Y, vdp_get_screen_width(), FONT_8X8_TOTAL_STORED_ROWS * 8, 0,
                                   DIX_RIGHT | DIY_DOWN, CMD_LOGIC_IMP);
}

void crt_font_8x8_print(const char ch, uint16_t x, uint16_t y) {
  const uint16_t from_x = crt_font_8x8_stored_x(ch);
  const uint16_t from_y = crt_font_8x8_stored_y(ch);
  printf("%x @ (%d, %d)\n", ch, from_x, from_y);

  vdp_cmd_wait_completion();
  vdp_cmd_logical_move_vram_to_vram(from_x, from_y, x, y, 8, 8, DIX_RIGHT | DIY_DOWN, CMD_LOGIC_REMAP);
}

#ifdef VDP_SUPER_HDMI

void crt_font_set_colours(uint8_t background_colour __attribute__((unused)), uint8_t foreground_colour __attribute__((unused))) {
  vdp_cmd_wait_completion();
  vdp_set_remap(background_colour, foreground_colour);
}

void test_logic_remap() {
  uint24_t from_x;
  uint24_t from_y;
  uint24_t ch;

  printf("FONT_8X8_MAX_SCREEN_LINES: %d\n", FONT_8X8_MAX_SCREEN_LINES);
  printf("FONT_8X8_MAX_SCREEN_ROWS: %d\n", FONT_8X8_MAX_SCREEN_ROWS);
  printf("FONT_8X8_TOTAL_STORED_ROWS: %d\n", FONT_8X8_TOTAL_STORED_ROWS);
  printf("FONT_8X8_STORED_Y: %d\n", FONT_8X8_STORED_Y);

  ch     = 0;
  from_x = crt_font_8x8_stored_x(ch);
  from_y = crt_font_8x8_stored_y(ch);
  printf("%x @ (%d, %d)\n", ch, from_x, from_y);

  ch     = 1;
  from_x = crt_font_8x8_stored_x(ch);
  from_y = crt_font_8x8_stored_y(ch);
  printf("%x @ (%d, %d)\n", ch, from_x, from_y);

  ch     = ' ';
  from_x = crt_font_8x8_stored_x(ch);
  from_y = crt_font_8x8_stored_y(ch);
  printf("%x @ (%d, %d)\n", ch, from_x, from_y);

  ch     = 'A';
  from_x = crt_font_8x8_stored_x(ch);
  from_y = crt_font_8x8_stored_y(ch);
  printf("%x @ (%d, %d)\n", ch, from_x, from_y);

  vdp_set_super_graphic_1();
  vdp_set_extended_palette(palette_256);

  vdp_cmd_wait_completion();
  vdp_cmd_logical_move_vdp_to_vram(0, 0, vdp_get_screen_width(), vdp_get_screen_height(), 0, DIX_RIGHT | DIY_DOWN, CMD_LOGIC_IMP);
  wait_for_key();

  crt_font_8x8_clear_patterns(); // selects an address at top of vram to store 256 8x8 font patterns
  crt_font_8x8_load_patterns(sysfont, ' ', sizeof(sysfont) / 8);

  printf("Patterns loaded\n");
  wait_for_key();

  crt_font_set_colours(1, 4);
  crt_font_8x8_print('A', 18, 20);

  crt_font_set_colours(2, 9);
  crt_font_8x8_print('C', 18 + 8, 24);

  for (uint24_t c = 0; c < 256; c++) {
    crt_font_set_colours(2, c);
    crt_font_8x8_print('C', 18 + 8, 24);

    wait_for_key();
  }
}
#endif
