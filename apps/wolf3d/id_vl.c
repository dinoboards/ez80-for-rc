// ID_VL.C

#include "crt.h"
#include "ez80-vdp.h"
#include "v99x8-hdmi/v99x8-wolf3d.h"
#include "wl_def.h"
#include <string.h>

// Uncomment the following line, if you get destination out of bounds
// assertion errors and want to ignore them during debugging
// #define IGNORE_BAD_DEST

#ifdef IGNORE_BAD_DEST
#undef assert
#define assert(x)                                                                                                                  \
  if (!(x))                                                                                                                        \
  return
#define assert_ret(x)                                                                                                              \
  if (!(x))                                                                                                                        \
  return 0
#else
#define assert_ret(x) assert(x)
#endif

boolean fullscreen = true;

boolean usedoublebuffering = true;

// ATSB: Global resolution for correct display of various things in the game like intermissions.
// ATSB: This goes along with the scaling to ensure it all looks nice.

unsigned screenBits = -1; // use "best" color depth according to libSDL

unsigned screenPitch;

boolean  screenfaded;
unsigned bordercolor;

#define CASSERT(x) extern int ASSERT_COMPILE[((x) != 0) * 2 - 1];

#define RGB(r, g, b) (g >> 3) << 5 | (r >> 3) << 2 | (b >> 4)

GRB_t gamepal[] __data_on_chip = {
#ifdef SPEAR
#include "sodpal.inc"
#else
#include "wolfpal.inc"
#endif
};

CASSERT(lengthof(gamepal) == 256)

//===========================================================================

/*
=======================
=
= VL_Shutdown
=
=======================
*/

/*
=============================================================================

                                                PALETTE OPS

                To avoid snow, do a WaitVBL BEFORE calling these

=============================================================================
*/

/*
=================
=
= VL_ConvertPalette
=
=================
*/

void VL_ConvertPalette(byte *srcpal, GRB_t *destpal, int numColors) {
  for (int i = 0; i < numColors; i++) {
    const uint8_t r = *srcpal++;
    const uint8_t g = *srcpal++;
    const uint8_t b = *srcpal++;
    destpal[i]      = GRB(g, r, b);
  }
}

//===========================================================================

//===========================================================================
//===========================================================================

/*
=================
=
= VL_SetPalette
=
=================
*/

void VL_SetPalette(GRB_t *palette __attribute__((unused)), bool forceupdate __attribute__((unused))) {
  printf("VL_SetPalette\r\n");
}

//===========================================================================

/*
=================
=
= VL_GetPalette
=
=================
*/

// void VL_GetPalette(GRB_t *palette) { memcpy(palette, curpal, sizeof(GRB_t) * 256); }

//===========================================================================

/*
=================
=
= VL_FadeOut
=
= Fades the current palette to the given color in the given number of steps
=
=================
*/

void VL_FadeOut(int start __attribute__((unused)),
                int end __attribute__((unused)),
                int red __attribute__((unused)),
                int green __attribute__((unused)),
                int blue __attribute__((unused)),
                int steps __attribute__((unused))) {
  // int        i, j, orig, delta;
  // GRB_t *origptr, *newptr;

  // red   = red * 255 / 63;
  // green = green * 255 / 63;
  // blue  = blue * 255 / 63;

  // VL_WaitVBL(1);
  // VL_GetPalette(palette1);
  // memcpy(palette2, palette1, sizeof(GRB_t) * 256);

  // //
  // // fade through intermediate frames
  // //
  // for (i = 0; i < steps; i++) {
  //   origptr = &palette1[start];
  //   newptr  = &palette2[start];
  //   for (j = start; j <= end; j++) {
  //     orig              = RED_FRM_GRB(*origptr);
  //     delta             = red - orig;
  //     uint8_t new_red   = orig + delta * i / steps;
  //     orig              = GREEN_FRM_GRB(*origptr);
  //     delta             = green - orig;
  //     uint8_t new_green = orig + delta * i / steps;
  //     orig              = BLUE_FRM_GRB(*origptr);
  //     delta             = blue - orig;
  //     uint8_t new_blue  = orig + delta * i / steps;

  //     *newptr = GRB(new_green, new_red, new_blue);
  //     origptr++;
  //     newptr++;
  //   }

  //   if (!usedoublebuffering || screenBits == 8)
  //     VL_WaitVBL(1);
  //   VL_SetPalette(palette2, true);
  // }

  // //
  // // final color
  // //
  // VL_FillPalette(red, green, blue);

  // screenfaded = true;
}

/*
=================
=
= VL_FadeIn
=
=================
*/

void VL_FadeIn(int    start __attribute__((unused)),
               int    end __attribute__((unused)),
               GRB_t *palette __attribute__((unused)),
               int    steps __attribute__((unused))) {
  // int i, j, delta;

  // VL_WaitVBL(1);
  // VL_GetPalette(palette1);
  // memcpy(palette2, palette1, sizeof(GRB_t) * 256);

  // //
  // // fade through intermediate frames
  // //
  // for (i = 0; i < steps; i++) {
  //   for (j = start; j <= end; j++) {
  //     delta         = palette[j].r - palette1[j].r;
  //     palette2[j].r = palette1[j].r + delta * i / steps;
  //     delta         = palette[j].g - palette1[j].g;
  //     palette2[j].g = palette1[j].g + delta * i / steps;
  //     delta         = palette[j].b - palette1[j].b;
  //     palette2[j].b = palette1[j].b + delta * i / steps;
  //   }

  //   if (!usedoublebuffering || screenBits == 8)
  //     VL_WaitVBL(1);
  //   VL_SetPalette(palette2, true);
  // }

  // //
  // // final color
  // //
  // VL_SetPalette(palette, true);
  // screenfaded = false;
}

/*
=================
=
= VL_Hlin
=
=================
*/
void VL_Hlin(unsigned x, unsigned y, unsigned width, uint8_t color) {
  // printf("VL_Hlin(%d, %d, %d)\r\n", x, y, width);

  assert(x >= 0 && "VL_Hlin: x < 0");
  assert(x + width <= SCREEN_WIDTH && "VL_Hlin: x + width > SCREEN_WIDTH!");
  assert(y >= 0 && "VL_Hlin: y < 0!");
  assert(y < SCREEN_HEIGHT && "VL_Hlin: y >= SCREEN_HEIGHT!");

  vdp_scn_h_line(x, y, width, color);
}

/*
=================
=
= VL_Vlin
=
=================
*/

void VL_Vlin(int x, int y, int height, int color) {
  // printf("VL_Vlin(%d, %d, %d)\r\n", x, y, height);

  assert(x >= 0 && "VL_Vlin: x < 0");
  assert((unsigned)x < SCREEN_WIDTH && "VL_Vlin: x >= SCREEN_WIDTH");
  assert(y >= 0 && "VL_Vlin: y < 0");
  assert((unsigned)y + height <= SCREEN_HEIGHT && "VL_Vlin: Destination rectangle out of bounds!");

  vdp_scn_v_line(x, y, height, color);
}

/*
=================
=
= VL_Bar
=
=================
*/

void VL_Bar(int scx, int scy, int scwidth, int scheight, uint8_t color) {
  // printf("VL_Bar(%d, %d, %d, %d, %d)\r\n", scx, scy, scwidth, scheight, color);
  assert(scx >= 0 && "VL_Bar: scx < 0!");
  assert((unsigned)scx + scwidth <= SCREEN_WIDTH && "VL_Bar: scx+width > SCREEN_WIDTH!");
  assert(scy >= 0 && "VL_Bar: scy < 0!");
  assert((unsigned)scy + scheight <= SCREEN_HEIGHT && "VL_Bar: scy+height > SCREEN_HEIGHT!");

  vdp_scn_bar(scx, scy, scwidth, scheight, color);
}

/*
============================================================================

                                                        MEMORY OPS

============================================================================
*/

/*
=================
=
= VL_MemToLatch
=
=================
*/

void VL_MemToLatch(byte *source, int width, int height, pixel_surface_t *destSurface, int x, int y) {
  assert(x >= 0 && (unsigned)x + width <= screenWidth && y >= 0 && (unsigned)y + height <= screenHeight &&
         "VL_MemToLatch: Destination rectangle out of bounds!");

  int   pitch = destSurface->width;
  byte *dest  = (byte *)destSurface->pixels + y * pitch + x;
  for (int ysrc = 0; ysrc < height; ysrc++) {
    for (int xsrc = 0; xsrc < width; xsrc++) {
      uint8_t color             = source[(ysrc * (width >> 2) + (xsrc >> 2)) + (xsrc & 3) * (width >> 2) * height];
      dest[ysrc * pitch + xsrc] = color;
    }
  }
}

//===========================================================================

/*
=================
=
= VL_MemToScreenScaledCoord
=
= Draws a block of data to the screen.
=
=================
*/

void VL_MemToScreen(const byte *source, int width, int height, int destx, int desty) {
  assert(destx >= 0 && destx + width <= 320 && desty >= 0 && desty + height <= 200 &&
         "VL_MemToScreenScaledCoord: Destination rectangle out of bounds!");

  int clipped_width = width;
  if (destx + width > SCREEN_WIDTH) {
    printf("Image Width clipped! VL_MemToScreen(%p, %d, %d, %d, %d)\r\n", source, width, height, destx, desty);

    clipped_width = SCREEN_WIDTH - destx;
    if (clipped_width <= 0) {
      printf("Image fully clipped\r\n");
      return;
    }
  }

  int clipped_height = height;
  if (desty + height > SCREEN_HEIGHT) {
    printf("Image Height clipped! VL_MemToScreen(%p, %d, %d, %d, %d)\r\n", source, width, height, destx, desty);
    clipped_height = SCREEN_HEIGHT - desty;

    if (clipped_height <= 0) {
      printf("Image fully clipped\r\n");
      return;
    }
  }

  vdp_scn_vga_picture(source, destx, desty, clipped_width, clipped_height, width);
}

//==========================================================================

/*
=================
=
= VL_LatchToScreen
=
=================
*/

void VL_SurfaceToScreen(pixel_surface_t *source, int scxdest, int scydest) {
  const byte    *src    = (byte *)source->pixels;
  const uint16_t width  = source->width;
  const uint16_t height = source->height;
  const uint16_t size   = source->size;

  assert(scxdest >= 0 && scxdest + width <= SCREEN_WIDTH && scydest >= 0 && scydest + height <= SCREEN_HEIGHT &&
         "VL_SurfaceToScreen: Destination rectangle out of bounds!");

  vdp_cmd_wait_completion();
  vdp_cmd_move_cpu_to_vram_with_palette(src, scxdest, scydest, width, height, 0, size, gamepal);
}

#ifdef APP_DEBUG
void VL_LatchToScreen(pixel_surface_t *source, int xsrc, int ysrc, int width, int height, int scxdest, int scydest) {
  printf("TODO: VL_LatchToScreen(");
  printf("source: %p, xsrc: %d, ysrc: %d, width: %d, height: %d, scxdest: %d, scydest: %d)\r\n", source, xsrc, ysrc, width, height,
         scxdest, scydest);
}
#endif

//===========================================================================
