// ID_VL.C

#include "crt.h"
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
unsigned screenWidth  = 320;
unsigned screenHeight = 200;

unsigned screenBits = -1; // use "best" color depth according to libSDL

SDL_Surface *screen = NULL;
unsigned     screenPitch;

SDL_Window *window = NULL;

SDL_Surface *screenBuffer = NULL;
unsigned     bufferPitch;

SDL_Renderer *renderer = NULL;

SDL_Surface *curSurface = NULL;
unsigned     curPitch;

// unsigned scaleFactor;

boolean  screenfaded;
unsigned bordercolor;

SDL_Color palette1[256], palette2[256];
SDL_Color curpal[256];

#define CASSERT(x) extern int ASSERT_COMPILE[((x) != 0) * 2 - 1];
#define RGB(r, g, b)                                                                                                               \
  { (r) * 255 / 63, (g)*255 / 63, (b)*255 / 63, 0 }

SDL_Color gamepal[] = {
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
  // #ifdef SPEAR
  // 	const char* title = "Spear of Destiny";
  // #else
  // 	const char* title = "Wolfenstein 3D";
  // #endif

  // 	//ATSB: Adjust W/H depending on current screen resolution
  // 	SDL_DisplayMode DispMode;
  // 	SDL_GetCurrentDisplayMode(0, &DispMode);

  // 	//Adjust height so the screen is 4:3 aspect ratio
  // 	//ATSB: Stuck in my DisplayMode as local variables for the SDL window and video scaling only
  // 	//ATSB: So that it looks better upscaled to a decent resolution, however all scaling
  // 	//ATSB: Remains at 4:3 320x200 so that nothing is stretched on widescreen resolutions.

  // 	screenWidth = DispMode.w * 3.0 / 4.0;
  // 	screenHeight = DispMode.h * 3.0 / 4.0;

  // 	window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight,
  // 		(fullscreen ? SDL_WINDOW_FULLSCREEN : 0) | SDL_WINDOW_OPENGL);

  // 	screen = SDL_GetWindowSurface(window);

  // 	if (!screen)
  // 	{
  // 		printf("Unable to set %ix%ix%i video mode: %s\n", screenWidth, screenHeight, screenBits, SDL_GetError());
  // 		exit(1);
  // 	}

  // 	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  // 	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  // 	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");

  // 	SDL_ShowCursor(SDL_DISABLE);

  // 	SDL_SetPaletteColors(screen->format->palette, gamepal, 0, 256);
  // 	memcpy(curpal, gamepal, sizeof(SDL_Color) * 256);

  // 	screenWidth = 320;
  // 	screenHeight = 200;

  // 	screenBuffer = SDL_CreateRGBSurface(0, screenWidth,
  // 		screenHeight, 8, 0, 0, 0, 0);
  // 	if (!screenBuffer)
  // 	{
  // 		printf("Unable to create screen buffer surface: %s\n", SDL_GetError());
  // 		exit(1);
  // 	}
  // 	SDL_SetPaletteColors(screenBuffer->format->palette, gamepal, 0, 256);

  // 	screenPitch = screen->pitch;
  // 	bufferPitch = screenBuffer->pitch;

  // 	curSurface = screenBuffer;
  // 	curPitch = bufferPitch;

  // 	scaleFactor = screenWidth / 320;
  // 	if (screenHeight / 200 < scaleFactor) scaleFactor = screenHeight / 200;

  // 	pixelangle = (short*)malloc(screenWidth * sizeof(short));
  // 	CHECKMALLOCRESULT(pixelangle);
  // 	wallheight = (int*)malloc(screenWidth * sizeof(int));
  // 	CHECKMALLOCRESULT(wallheight);
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
    destpal[i].r = *srcpal++ * 255 / 63;
    destpal[i].g = *srcpal++ * 255 / 63;
    destpal[i].b = *srcpal++ * 255 / 63;
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
  // 	SDL_SetPaletteColors(curSurface->format->palette, &col, color, 1);
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

void VL_GetColor(int color, int *red, int *green, int *blue) {
  SDL_Color *col = &curpal[color];
  *red           = col->r;
  *green         = col->g;
  *blue          = col->b;
}

//===========================================================================

/*
=================
=
= VL_SetPalette
=
=================
*/

void VL_SetPalette(SDL_Color *palette, bool forceupdate __attribute__((unused))) {
  memcpy(curpal, palette, sizeof(SDL_Color) * 256);

  // if (screenBits == 8)
  // 	SDL_SetPaletteColors(screen->format->palette, palette, 0, 256);
  // else
  // {
  // 	SDL_SetPaletteColors(curSurface->format->palette, palette, 0, 256);
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

void VL_GetPalette(SDL_Color *palette) { memcpy(palette, curpal, sizeof(SDL_Color) * 256); }

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
  // int         i, j, orig, delta;
  // SDL_Color* origptr, * newptr;

  // red = red * 255 / 63;
  // green = green * 255 / 63;
  // blue = blue * 255 / 63;

  // VL_WaitVBL(1);
  // VL_GetPalette(palette1);
  // memcpy(palette2, palette1, sizeof(SDL_Color) * 256);

  // //
  // // fade through intermediate frames
  // //
  // for (i = 0; i < steps; i++)
  // {
  // 	origptr = &palette1[start];
  // 	newptr = &palette2[start];
  // 	for (j = start; j <= end; j++)
  // 	{
  // 		orig = origptr->r;
  // 		delta = red - orig;
  // 		newptr->r = orig + delta * i / steps;
  // 		orig = origptr->g;
  // 		delta = green - orig;
  // 		newptr->g = orig + delta * i / steps;
  // 		orig = origptr->b;
  // 		delta = blue - orig;
  // 		newptr->b = orig + delta * i / steps;
  // 		origptr++;
  // 		newptr++;
  // 	}

  // 	if (!usedoublebuffering || screenBits == 8) VL_WaitVBL(1);
  // 	VL_SetPalette(palette2, true);
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
=============================================================================

                                                        PIXEL OPS

=============================================================================
*/

byte *VL_LockSurface(SDL_Surface *surface) {
  // if (SDL_MUSTLOCK(surface))
  // {
  // 	if (SDL_LockSurface(surface) < 0)
  // 		return NULL;
  // }
  return (byte *)surface->pixels;
}

void VL_UnlockSurface(SDL_Surface *surface __attribute__((unused))) {
  // if (SDL_MUSTLOCK(surface))
  // {
  // 	SDL_UnlockSurface(surface);
  // }
}

/*
=================
=
= VL_Plot
=
=================
*/

void VL_Plot(int x __attribute__((unused)), int y __attribute__((unused)), int color __attribute__((unused))) {
  // assert(x >= 0 && (unsigned)x < screenWidth && y >= 0 && (unsigned)y < screenHeight && "VL_Plot: Pixel out of bounds!");

  // VL_LockSurface(curSurface);
  // ((byte *)curSurface->pixels)[y * curPitch + x] = color;
  // VL_UnlockSurface(curSurface);
}

/*
=================
=
= VL_GetPixel
=
=================
*/

byte VL_GetPixel(int x __attribute__((unused)), int y __attribute__((unused))) {
  // assert_ret(x >= 0 && (unsigned)x < screenWidth && y >= 0 && (unsigned)y < screenHeight && "VL_GetPixel: Pixel out of bounds!");

  // VL_LockSurface(curSurface);
  // byte col = ((byte *)curSurface->pixels)[y * curPitch + x];
  // VL_UnlockSurface(curSurface);
  // return col;

  return 0;
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

  // VL_LockSurface(curSurface);
  // uint8_t* dest = ((byte*)curSurface->pixels) + y * curPitch + x;
  // memset(dest, color, width);
  // VL_UnlockSurface(curSurface);
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

  // VL_LockSurface(curSurface);
  // uint8_t *dest = ((byte *)curSurface->pixels) + y * curPitch + x;

  // while (height--) {
  //   *dest = color;
  //   dest += curPitch;
  // }
  // VL_UnlockSurface(curSurface);
}

/*
=================
=
= VL_Bar
=
=================
*/

void VL_BarScaledCoord(int scx __attribute__((unused)),
                       int scy __attribute__((unused)),
                       int scwidth __attribute__((unused)),
                       int scheight __attribute__((unused)),
                       int color __attribute__((unused))) {
  // assert(scx >= 0 && (unsigned)scx + scwidth <= screenWidth && scy >= 0 && (unsigned)scy + scheight <= screenHeight &&
  //        "VL_BarScaledCoord: Destination rectangle out of bounds!");

  // VL_LockSurface(curSurface);
  // uint8_t *dest = ((byte *)curSurface->pixels) + scy * curPitch + scx;

  // while (scheight--) {
  //   memset(dest, color, scwidth);
  //   dest += curPitch;
  // }
  // VL_UnlockSurface(curSurface);
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

void VL_MemToLatch(byte        *source __attribute__((unused)),
                   int          width __attribute__((unused)),
                   int          height __attribute__((unused)),
                   SDL_Surface *destSurface __attribute__((unused)),
                   int          x __attribute__((unused)),
                   int          y __attribute__((unused))) {
  // assert(x >= 0 && (unsigned)x + width <= screenWidth && y >= 0 && (unsigned)y + height <= screenHeight &&
  //        "VL_MemToLatch: Destination rectangle out of bounds!");

  // VL_LockSurface(destSurface);
  // int   pitch = destSurface->pitch;
  // byte *dest  = (byte *)destSurface->pixels + y * pitch + x;
  // for (int ysrc = 0; ysrc < height; ysrc++) {
  //   for (int xsrc = 0; xsrc < width; xsrc++) {
  //     dest[ysrc * pitch + xsrc] = source[(ysrc * (width >> 2) + (xsrc >> 2)) + (xsrc & 3) * (width >> 2) * height];
  //   }
  // }
  // VL_UnlockSurface(destSurface);
}

//===========================================================================

/*
=================
=
= VL_MemToScreenScaledCoord
=
= Draws a block of data to the screen with scaling according to scaleFactor.
=
=================
*/

// TODO: Change this to work with the V9958
void VL_MemToScreenScaledCoordA(byte *source, int width, int height, int destx, int desty) {
  printf("VL_MemToScreenScaledCoordA\r\n");
  printf("source: %p, width: %d, height: %d, destx: %d, desty: %d\r\n", source, width, height, destx, desty);

  assert(destx >= 0 && destx + width * scaleFactor <= screenWidth && desty >= 0 && desty + height * scaleFactor <= screenHeight &&
         "VL_MemToScreenScaledCoord: Destination rectangle out of bounds!");

  // VL_LockSurface(curSurface);
  // byte *vbuf = (byte *)curSurface->pixels;
  // for (int j = 0, scj = 0; j < height; j++, scj += scaleFactor) {
  //   for (int i = 0, sci = 0; i < width; i++, sci += scaleFactor) {
  //     byte col = source[(j * (width >> 2) + (i >> 2)) + (i & 3) * (width >> 2) * height];
  //     for (unsigned m = 0; m < scaleFactor; m++) {
  //       for (unsigned n = 0; n < scaleFactor; n++) {
  //         vbuf[(scj + m + desty) * curPitch + sci + n + destx] = col;
  //       }
  //     }
  //   }
  // }
  // VL_UnlockSurface(curSurface);
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

  assert(destx >= 0 && destx + width * scaleFactor <= screenWidth && desty >= 0 && desty + height * scaleFactor <= screenHeight &&
         "VL_MemToScreenScaledCoord: Destination rectangle out of bounds!");

  // VL_LockSurface(curSurface);
  // byte *vbuf = (byte *)curSurface->pixels;
  // for (int j = 0, scj = 0; j < height; j++, scj += scaleFactor) {
  //   for (int i = 0, sci = 0; i < width; i++, sci += scaleFactor) {
  //     byte col = source[((j + srcy) * (origwidth >> 2) + ((i + srcx) >> 2)) + ((i + srcx) & 3) * (origwidth >> 2) * origheight];
  //     for (unsigned m = 0; m < scaleFactor; m++) {
  //       for (unsigned n = 0; n < scaleFactor; n++) {
  //         vbuf[(scj + m + desty) * curPitch + sci + n + destx] = col;
  //       }
  //     }
  //   }
  // }
  // VL_UnlockSurface(curSurface);
}

//==========================================================================

/*
=================
=
= VL_LatchToScreen
=
=================
*/

void VL_LatchToScreenScaledCoord7(SDL_Surface *source, int xsrc, int ysrc, int width, int height, int scxdest, int scydest) {

  printf("VL_LatchToScreenScaledCoord7\r\n");
  printf("source: %p, xsrc: %d, ysrc: %d, width: %d, height: %d, scxdest: %d, scydest: %d\r\n", source, xsrc, ysrc, width, height,
         scxdest, scydest);

  assert(scxdest >= 0 && scxdest + width * scaleFactor <= screenWidth && scydest >= 0 &&
         scydest + height * scaleFactor <= screenHeight && "VL_LatchToScreenScaledCoord7: Destination rectangle out of bounds!");

  // if (scaleFactor == 1) {
  //   VL_LockSurface(source);
  //   byte    *src      = (byte *)source->pixels;
  //   unsigned srcPitch = source->pitch; // number of bytes to be added, to get to next row

  //   VL_LockSurface(curSurface);
  //   byte *vbuf = (byte *)curSurface->pixels;
  //   for (int j = 0; j < height; j++) {
  //     for (int i = 0; i < width; i++) {

  //       byte col = src[(ysrc + j) * srcPitch + xsrc + i];

  //       vbuf[(scydest + j) * curPitch + scxdest + i] = col;
  //     }
  //   }
  //   VL_UnlockSurface(curSurface);
  //   VL_UnlockSurface(source);
  // }
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
