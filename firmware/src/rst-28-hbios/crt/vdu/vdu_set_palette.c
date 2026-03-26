#include <ez80.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <v99x8.h>
#include <vdu-standards-colors.h>

extern const RGB default_2_colour_palette[16];
extern const RGB default_4_colour_palette[16];
extern const RGB default_16_colour_palette[16];

static void _vdp_set_palette(const RGB *pPalette) {
  uint8_t c;
#pragma asm DI
  for (c = 0; c < 16; c++) {
    vdp_reg_write(16, c);
    vdp_out_pal((pPalette->red & 7) * 16 + (pPalette->blue & 7));
    vdp_out_pal(pPalette->green & 7);
    pPalette++;
  }
#pragma asm EI
}

void vdu_set_default_palette_2(void) { _vdp_set_palette(default_2_colour_palette); }

void vdu_set_default_palette_4(void) { _vdp_set_palette(default_4_colour_palette); }

void vdu_set_default_palette_16(void) { _vdp_set_palette(default_16_colour_palette); }
