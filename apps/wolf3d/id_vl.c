// ID_VL.C

#include "crt.h"
#include "wl_def.h"
#include <string.h>
#include <v99x8.h>

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
// const uint24_t screenWidth  = 256;
// const uint24_t screenHeight = 212;

unsigned screenBits = -1; // use "best" color depth according to libSDL

SDL_Surface *screen = NULL;
unsigned     screenPitch;

SDL_Window *window = NULL;

SDL_Surface *screenBuffer = NULL;

SDL_Renderer *renderer = NULL;

boolean  screenfaded;
unsigned bordercolor;

#define CASSERT(x) extern int ASSERT_COMPILE[((x) != 0) * 2 - 1];
// #define RGB(r, g, b)                                                                                                               \
//   { (r) * 255 / 63, (g)*255 / 63, (b)*255 / 63, 0 }

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

void VL_Shutdown(void) {
  // SDL_FreeSurface(screen);
  // SDL_FreeSurface(screenBuffer);
  // for (auto& surface : latchpics) {
  // 	SDL_FreeSurface(surface);
  // }
  // SDL_DestroyRenderer(renderer);
  // SDL_DestroyWindow(window);
}

/*
=======================
=
= VL_SetVGAPlaneMode
=
=======================
*/

void VL_SetVGAPlaneMode(void) {
  vdp_set_mode(7, 212, PAL);
  vdp_cmd_wait_completion();
  vdp_cmd_vdp_to_vram(0, 0, 256, 212, 0, 0);
  vdp_cmd_wait_completion();

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

void VL_Hlin(unsigned x __attribute__((unused)),
             unsigned y __attribute__((unused)),
             unsigned width __attribute__((unused)),
             int      color __attribute__((unused))) {
  // assert(x >= 0 && x + width <= screenWidth
  // 	&& y >= 0 && y < screenHeight
  // 	&& "VL_Hlin: Destination rectangle out of bounds!");

  printf("TODO!!! VL_Hlin\r\n");
  // uint8_t* dest = ((byte*)screenBuffer->pixels) + y * SCREEN_WIDTH + x;
  // memset(dest, color, width);
}

/*
=================
=
= VL_Vlin
=
=================
*/

void VL_Vlin(int x __attribute__((unused)),
             int y __attribute__((unused)),
             int height __attribute__((unused)),
             int color __attribute__((unused))) {
  // assert(x >= 0 && (unsigned)x < screenWidth && y >= 0 && (unsigned)y + height <= screenHeight &&
  //        "VL_Vlin: Destination rectangle out of bounds!");

  printf("TODO!!! VL_Vlin\r\n");
  // uint8_t *dest = ((byte *)screenBuffer->pixels) + y * SCREEN_WIDTH + x;

  // while (height--) {
  //   *dest = color;
  //   dest += SCREEN_WIDTH;
  // }
}

/*
=================
=
= VL_Bar
=
=================
*/

void VL_BarScaledCoord(int scx, int scy, int scwidth, int scheight, int color) {
  assert(scx >= 0 && (unsigned)scx + scwidth <= screenWidth && scy >= 0 && (unsigned)scy + scheight <= screenHeight &&
         "VL_BarScaledCoord: Destination rectangle out of bounds!");

  uint8_t *dest = ((byte *)screenBuffer->xpixels) + scy * SCREEN_WIDTH + scx;

  while (scheight--) {
    memset(dest, color, scwidth);
    dest += SCREEN_WIDTH;
  }
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

void VL_MemToScreen(byte *source, int width, int height, int destx, int desty) {

  assert(destx >= 0 && destx + width <= (int)320 && desty >= 0 && desty + height <= (int)screenHeight &&
         "VL_MemToScreenScaledCoord: Destination rectangle out of bounds!");

  byte *vbuf = (byte *)screenBuffer->xpixels;
  for (int j = 0; j < height; j++) {
    for (int i = 0; i < width; i++) {
      const byte color = source[(j * (width >> 2) + (i >> 2)) + (i & 3) * (width >> 2) * height];

      const uint8_t xx = scale_points[i + destx]; // TODO: this is sub-optimal - as it processes the same pixel multiple times
      vbuf[(j + desty) * SCREEN_WIDTH + xx] = color;
    }
  }
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

  assert(destx >= 0 && destx + width * scaleFactor <= (int)screenWidth && desty >= 0 &&
         desty + height * scaleFactor <= (int)screenHeight && "VL_MemToScreenScaledCoord: Destination rectangle out of bounds!");

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

void VL_LatchToScreen(SDL_Surface *source, int xsrc, int ysrc, int width, int height, int scxdest, int scydest) {
  // printf("VL_LatchToScreen(");
  // printf("source: %p, xsrc: %d, ysrc: %d, width: %d, height: %d, scxdest: %d, scydest: %d)\r\n", source, xsrc, ysrc, width,
  // height,
  //        scxdest, scydest);

  assert(scxdest >= 0 && scxdest + width <= SCREEN_WIDTH && scydest >= 0 && scydest + height <= (int)screenHeight &&
         "VL_LatchToScreen: Destination rectangle out of bounds!");

  byte    *src      = (byte *)source->xpixels;
  uint24_t srcPitch = source->pitch; // number of bytes to be added, to get to next row

  byte *vbuf = (byte *)screenBuffer->xpixels;

  for (int j = 0; j < height; j++) {
    for (int i = 0; i < width; i++) {
      byte color = src[(ysrc + j) * srcPitch + xsrc + i];

      const uint8_t xx                        = i + scxdest;
      vbuf[(scydest + j) * SCREEN_WIDTH + xx] = color;
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
