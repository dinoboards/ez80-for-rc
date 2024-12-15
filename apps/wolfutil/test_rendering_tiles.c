#include <cpm.h>
#include <stdint.h>
#include <stdio.h>
#include <v99x8.h>

#include "wolfutil.h"

extern uint8_t tiles[];

// width: 48, height 24
extern uint8_t kifePic[];
extern uint8_t gunpick[];

void apply_palette(uint8_t *surface, uint16_t width, uint16_t height) {

  uint8_t *c = surface;

  for (uint8_t y = 0; y < height; y++) {
    for (uint16_t x = 0; x < width; x++) {
      *c = gamepal[*c];
      c++;
    }
  }
}

uint8_t buffer[48 * 24];

void transform(uint8_t *source, int width, int height, uint8_t *destSurface, int x, int y) {
  uint8_t *dest = (uint8_t *)destSurface + y * width + x;
  for (int ysrc = 0; ysrc < height; ysrc++) {
    for (int xsrc = 0; xsrc < width; xsrc++) {
      dest[ysrc * width + xsrc] = source[(ysrc * (width >> 2) + (xsrc >> 2)) + (xsrc & 3) * (width >> 2) * height];
    }
  }
}

void test_rendering_tiles() {
  printf("gamepal is at %p\r\n", gamepal);

  vdp_set_mode(7, 212, PAL);
  printf("Erase screen\r\n");
  vdp_cmd_vdp_to_vram(0, 0, 256, 212, 00, 0);

  printf("Erase screen done\r\nPress key to use fast version");

  while (cpm_c_rawio() == 0)
    ;

  vdp_cmd_vdp_to_vram(0, 0, 256, 212, 55, 0);

  while (cpm_c_rawio() == 0)
    ;

  printf("Done\r\n");

  uint8_t x = 0;
  uint8_t y = 0;

  for (int i = 0; i < 70; i++) {
    uint8_t *c = tiles;

    apply_palette(c, 8, 8);

    transform(c, 8, 8, buffer, 0, 0);

    vdp_cmd_move_cpu_to_vram(buffer, x, y, 8, 8, 0, 8 * 8);

    x += 8;
    if (x == 0) {
      y += 8;
      while (cpm_c_rawio() == 0)
        ;
    }

    if (y >= 210)
      break;
  }
}
