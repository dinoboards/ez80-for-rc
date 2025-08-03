#include <ez80.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <v99x8-super.h>

void vdp_set_extended_palette(RGB *pPalette) {
  uint16_t c;
  DI();
  register_31_mirror |= 0x08;
  vdp_reg_write(31, register_31_mirror);
  for (c = 0; c < 256; c++) {
    vdp_reg_write(16, ((uint8_t)c));
    vdp_out_pal(pPalette->red);
    vdp_out_pal(pPalette->green);
    vdp_out_pal(pPalette->blue);
    pPalette++;
  }

  register_31_mirror &= ~0x08;
  vdp_reg_write(31, register_31_mirror);

  EI();
}
