#include "crt.h"
#include <stdint.h>
#include <v99x8-super.h>

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

void crt_font_8x8_load_patterns(const uint8_t *const font_patterns, const uint8_t start_ch, const uint8_t number_of_ch) {
  uint24_t       gpos_x = crt_font_8x8_stored_x(start_ch);
  uint24_t       gpos_y = crt_font_8x8_stored_y(start_ch);
  const uint8_t *p      = &font_patterns[0];
  const uint24_t length = number_of_ch == 0 ? 256 : (uint24_t)number_of_ch;
  uint24_t       ch;
  uint8_t        x;
  uint8_t        y;
  // DI();

  vdp_reg_write(45, 0);

  for (ch = start_ch; ch < length; ch++) {
    for (y = 0; y < 8; y++) {
      const uint8_t r = *p++;

      vdp_reg_write(38, gpos_y & 0xFF);
      vdp_reg_write(39, (gpos_y >> 8) & 255);

      for (x = 0; x < 8; x++) {
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

  // EI();-0
}

void crt_font_8x8_clear_patterns() {
  vdp_cmd_wait_completion();
  vdp_cmd_logical_move_vdp_to_vram(0, FONT_8X8_STORED_Y, FONT_8X8_PER_ROW * 8, FONT_8X8_TOTAL_STORED_ROWS * 8, 0,
                                   DIX_RIGHT | DIY_DOWN, CMD_LOGIC_IMP);
}

void crt_font_8x8_print(const char ch, const uint16_t x, const uint16_t y) {
  const uint16_t from_x = crt_font_8x8_stored_x(ch);
  const uint16_t from_y = crt_font_8x8_stored_y(ch);

  vdp_cmd_wait_completion();
  vdp_cmd_logical_move_vram_to_vram(from_x, from_y, x, y, 8, 8, DIX_RIGHT | DIY_DOWN, CMD_LOGIC_REMAP);
}

void crt_font_set_colours(const uint8_t background_colour, const uint8_t foreground_colour) {
  vdp_cmd_wait_completion();
  vdp_set_remap(background_colour, foreground_colour);
}

// extern const RGB palette_256[256];

// void crt_spike() {

//   vdp_set_super_graphic_1();
//   vdp_set_extended_palette(palette_256);

//   vdp_cmd_wait_completion();
//   vdp_cmd_logical_move_vdp_to_vram(0, 0, vdp_get_screen_width(), vdp_get_screen_height(), 0, DIX_RIGHT | DIY_DOWN,
//   CMD_LOGIC_IMP);

//   crt_font_8x8_clear_patterns(); // selects an address at top of vram to store 256 8x8 font patterns
//   crt_font_8x8_load_patterns(sysfont, ' ', sizeof(sysfont) / 8);

//   crt_font_set_colours(1, 4);
//   crt_font_8x8_print('A', 18, 20);

//   crt_font_set_colours(2, 9);
//   crt_font_8x8_print('C', 18 + 8, 24);
// }
