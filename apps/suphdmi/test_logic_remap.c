#include "palettes.h"
#include "wait_for_key.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <v99x8-super.h>

typedef struct text_colours {
  uint8_t fg;
  uint8_t bg;
} text_colours_t;

extern uint8_t sysfont[(128 - ' ') * 8]; // 96*8

uint8_t        font_patterns[256 * 8];
text_colours_t font_color[256];
uint8_t        current_tbg_colour = 0;
uint8_t        current_tfg_colour = 1;

void init_font_patterns() {
  memset(font_patterns, 0, 256 * 8);

  memcpy(&font_patterns[' ' * 8], sysfont, sizeof(sysfont));

  for (int i = 0; i < 256; i++) {
    font_color[i].fg = 255;
    font_color[i].bg = 255;
  }
}

void prepare_font_pattern(uint8_t ch, uint16_t gpos_x, uint16_t gpos_y) {
  uint8_t *p        = &font_patterns[ch * 8];
  font_color[ch].fg = current_tfg_colour;
  font_color[ch].bg = current_tbg_colour;

  vdp_cmd_wait_completion();
  vdp_cmd_logical_move_vdp_to_vram(gpos_x, gpos_y, 8, 8, current_tbg_colour, DIX_RIGHT | DIY_DOWN, CMD_LOGIC_IMP);

  printf("Pattern for %c\n", ch);

  for (int y = 0; y < 8; y++) {
    const uint8_t r = *p++;
    for (int x = 0; x < 8; x++) {
      const bool pixel_on = (r & (1 << (7 - x)));
      if (pixel_on) {
        vdp_cmd_wait_completion();
        vdp_cmd_pset(gpos_x, gpos_y, current_tfg_colour, 0);
        printf("X");
      } else
        printf(" ");
      gpos_x++;
    }

    printf("\n");
    gpos_x -= 8;
    gpos_y++;
  }
}

#ifdef VDP_SUPER_HDMI

#define FONT_Y_OFFSET 128
// ((576 * 2))

void test_logic_remap() {
  vdp_set_super_graphic_10();
  vdp_set_extended_palette(palette_256);

  vdp_cmd_wait_completion();
  vdp_cmd_logical_move_vdp_to_vram(0, 0, vdp_get_screen_width(), vdp_get_screen_height(), 0, DIX_RIGHT | DIY_DOWN, CMD_LOGIC_IMP);

  init_font_patterns();

  uint16_t from_x;
  uint16_t from_y;
  char     ch;

  ch     = 'A';
  from_x = (ch % 32) * 8;
  from_y = FONT_Y_OFFSET + (ch / 32) * 8;
  prepare_font_pattern(ch, from_x, from_y);
  vdp_cmd_wait_completion();
  vdp_cmd_logical_move_vram_to_vram(from_x, from_y, 16, 16, 8, 8, DIX_RIGHT | DIY_DOWN, CMD_LOGIC_IMP);

  ch                 = 'C';
  current_tfg_colour = 2;
  from_x             = (ch % 32) * 8;
  from_y             = FONT_Y_OFFSET + (ch / 32) * 8;
  prepare_font_pattern(ch, from_x, from_y);
  vdp_cmd_wait_completion();
  vdp_cmd_logical_move_vram_to_vram(from_x, from_y, 16 + 8, 16, 8, 8, DIX_RIGHT | DIY_DOWN, CMD_LOGIC_IMP);

  wait_for_key();
}
#endif
