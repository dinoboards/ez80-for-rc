#include <ez80.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../v99x8.h"

void vdp_set_palette(RGB *pPalette) {
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
