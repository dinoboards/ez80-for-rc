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

SDL_Surface *screen = NULL;
unsigned     screenPitch;

SDL_Window *window = NULL;

SDL_Surface *screenBuffer = NULL;

SDL_Renderer *renderer = NULL;

boolean  screenfaded;
unsigned bordercolor;

#define CASSERT(x) extern int ASSERT_COMPILE[((x) != 0) * 2 - 1];

#define RGB(r, g, b) (g >> 3) << 5 | (r >> 3) << 2 | (b >> 4)

SDL_Color gamepal[] __data_on_chip = {
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

void VL_Shutdown(void) {}

/*
=======================
=
= VL_InitVideoMode
=
=======================
*/

void VL_InitVideoMode(void) {
  vdp_scn_init();

  screenBuffer = SDL_CreateRGBSurface(screenWidth, screenHeight);
}

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

void VL_ConvertPalette(byte *srcpal, SDL_Color *destpal, int numColors) {
  for (int i = 0; i < numColors; i++) {
    const uint8_t r = *srcpal++;
    const uint8_t g = *srcpal++;
    const uint8_t b = *srcpal++;
    destpal[i]      = GRB(g, r, b);
  }
}

/*
=================
=
= VL_FillPalette
=
=================
*/

void VL_FillPalette(int red __attribute__((unused)), int green __attribute__((unused)), int blue __attribute__((unused))) {
  // int i;
  // SDL_Color pal[256];

  // for (i = 0; i < 256; i++)
  // {
  // 	pal[i].r = red;
  // 	pal[i].g = green;
  // 	pal[i].b = blue;
  // }

  // VL_SetPalette(pal, true);
}

//===========================================================================

/*
=================
=
= VL_SetColor
=
=================
*/

void VL_SetColor(int color __attribute__((unused)),
                 int red __attribute__((unused)),
                 int green __attribute__((unused)),
                 int blue __attribute__((unused))) {
  // SDL_Color col = { static_cast<uint8_t>(red), static_cast<uint8_t>(green), static_cast<uint8_t>(blue) };
  // curpal[color] = col;

  // if (screenBits == 8)
  // 	SDL_SetPaletteColors(screen->format->palette, &col, color, 1);
  // else
  // {
  // 	SDL_SetPaletteColors(screenBuffer->format->palette, &col, color, 1);
  // 	SDL_BlitSurface(screenBuffer, NULL, screen, NULL);
  // 	SDL_Flip(screen);
  // }
}

//===========================================================================

/*
=================
=
= VL_GetColor
=
=================
*/

// void VL_GetColor(int color, int *red, int *green, int *blue) {
//   SDL_Color *col = &curpal[color];
//   *red           = RED_FRM_GRB(col);
//   *green         = GREEN_FRM_GRB(col);
//   *blue          = BLUE_FRM_GRB(col);
// }

//===========================================================================

/*
=================
=
= VL_SetPalette
=
=================
*/

void VL_SetPalette(SDL_Color *palette __attribute__((unused)), bool forceupdate __attribute__((unused))) {
  // memcpy(curpal, palette, sizeof(SDL_Color) * 256);

  // if (screenBits == 8)
  // 	SDL_SetPaletteColors(screen->format->palette, palette, 0, 256);
  // else
  // {
  // 	SDL_SetPaletteColors(screenBuffer->format->palette, palette, 0, 256);
  // 	if (forceupdate)
  // 	{
  // 		SDL_BlitSurface(screenBuffer, NULL, screen, NULL);
  // 		SDL_Flip(screen);
  // 	}
  // }
}

//===========================================================================

/*
=================
=
= VL_GetPalette
=
=================
*/

// void VL_GetPalette(SDL_Color *palette) { memcpy(palette, curpal, sizeof(SDL_Color) * 256); }

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
  // SDL_Color *origptr, *newptr;

  // red   = red * 255 / 63;
  // green = green * 255 / 63;
  // blue  = blue * 255 / 63;

  // VL_WaitVBL(1);
  // VL_GetPalette(palette1);
  // memcpy(palette2, palette1, sizeof(SDL_Color) * 256);

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

void VL_FadeIn(int        start __attribute__((unused)),
               int        end __attribute__((unused)),
               SDL_Color *palette __attribute__((unused)),
               int        steps __attribute__((unused))) {
  // int i, j, delta;

  // VL_WaitVBL(1);
  // VL_GetPalette(palette1);
  // memcpy(palette2, palette1, sizeof(SDL_Color) * 256);

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
= VL_Plot
=
=================
*/

void VL_Plot(int x, int y, int color) {
  assert(x >= 0 && (unsigned)x < screenWidth && y >= 0 && (unsigned)y < screenHeight && "VL_Plot: Pixel out of bounds!");

  screenBuffer->xpixels[y * SCREEN_WIDTH + x] = color;
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

  uint8_t *dest = ((byte *)screenBuffer->xpixels) + y * SCREEN_WIDTH + x;
  memset(dest, color, width);
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

  uint8_t *dest = ((byte *)screenBuffer->xpixels) + y * SCREEN_WIDTH + x;

  while (height--) {
    *dest = color;
    dest += SCREEN_WIDTH;
  }
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

void VL_MemToLatch(byte *source, int width, int height, SDL_Surface *destSurface, int x, int y) {
  assert(x >= 0 && (unsigned)x + width <= screenWidth && y >= 0 && (unsigned)y + height <= screenHeight &&
         "VL_MemToLatch: Destination rectangle out of bounds!");

  int   pitch = destSurface->pitch;
  byte *dest  = (byte *)destSurface->xpixels + y * pitch + x;
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

/*
=================
=
= VL_MemToScreenScaledCoord
=
= Draws a part of a block of data to the screen.
= The block has the size origwidth*origheight.
= The part at (srcx, srcy) has the size width*height
= and will be painted to (destx, desty) with scaling according to scaleFactor.
=
=================
*/

void VL_MemToScreenScaledCoordN(
    byte *source, int origwidth, int origheight, int srcx, int srcy, int destx, int desty, int width, int height) {

  printf("VL_MemToScreenScaledCoordN\r\n");
  printf("source: %p, origwidth: %d, origheight: %d, srcx: %d, srcy: %d, destx: %d, desty: %d, width: %d, height: %d\r\n", source,
         origwidth, origheight, srcx, srcy, destx, desty, width, height);

  // assert(destx >= 0 && destx + width * scaleFactor <= (int)screenWidth && desty >= 0 &&
  //        desty + height * scaleFactor <= (int)screenHeight && "VL_MemToScreenScaledCoord: Destination rectangle out of bounds!");

  printf("TODO!!!! VL_MemToScreenScaledCoordN\r\n");
  // byte *vbuf = (byte *)screenBuffer->pixels;
  // for (int j = 0, scj = 0; j < height; j++, scj += scaleFactor) {
  //   for (int i = 0, sci = 0; i < width; i++, sci += scaleFactor) {
  //     byte col = source[((j + srcy) * (origwidth >> 2) + ((i + srcx) >> 2)) + ((i + srcx) & 3) * (origwidth >> 2) * origheight];
  //     for (unsigned m = 0; m < scaleFactor; m++) {
  //       for (unsigned n = 0; n < scaleFactor; n++) {
  //         vbuf[(scj + m + desty) * SCREEN_WIDTH + sci + n + destx] = col;
  //       }
  //     }
  //   }
  // }
}

//==========================================================================

/*
=================
=
= VL_LatchToScreen
=
=================
*/

void VL_SurfaceToScreen(SDL_Surface *source, int scxdest, int scydest) {
  const byte *src    = (byte *)source->xpixels;
  const int   width  = source->w;
  const int   height = source->h;

  assert(scxdest >= 0 && scxdest + width <= SCREEN_WIDTH && scydest >= 0 && scydest + height <= SCREEN_HEIGHT &&
         "VL_SurfaceToScreen: Destination rectangle out of bounds!");

  vdp_cmd_wait_completion();
  vdp_cmd_move_cpu_to_vram_with_palette(src, scxdest, scydest, width, height, 0, width * height, gamepal);
}

void VL_LatchToScreen(SDL_Surface *source, int xsrc, int ysrc, int width, int height, int scxdest, int scydest) {
  // printf("VL_LatchToScreen(");
  // printf("source: %p, xsrc: %d, ysrc: %d, width: %d, height: %d, scxdest: %d, scydest: %d)\r\n", source, xsrc, ysrc, width,
  // height,
  //        scxdest, scydest);

  assert(scxdest >= 0 && scxdest + width <= SCREEN_WIDTH && scydest >= 0 && scydest + height <= (int)screenHeight &&
         "VL_LatchToScreen: Destination rectangle out of bounds!");

  byte    *src      = (byte *)source->xpixels;
  uint24_t srcPitch = source->pitch; // number of bytes to be added, to get to next row

  uint8_t first_byte = src[(ysrc)*srcPitch + xsrc];

  vdp_cmd_wait_completion();
  vdp_cmd_move_data_to_vram(gamepal[first_byte], scxdest, scydest, width, height, 0, width * height);

  for (int j = 0; j < height; j++) {
    for (int i = 0; i < width; i++) {
      if (i == 0 && j == 0) {
        continue;
      }

      byte    color = src[(ysrc + j) * srcPitch + xsrc + i];
      uint8_t grb   = gamepal[color];

      vdp_cmd_send_byte(grb);
    }
  }
}

//===========================================================================

/*
=================
=
= VL_ScreenToScreen
=
=================
*/

void VL_ScreenToScreen(SDL_Surface *source __attribute__((unused)), SDL_Surface *dest __attribute__((unused))) {
  printf("VL_ScreenToScreen\r\n");
  printf("source: %p, dest: %p\r\n", source, dest);
  // SDL_BlitSurface(source, NULL, dest, NULL);
}
