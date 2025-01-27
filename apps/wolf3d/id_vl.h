// ID_VL.H

// wolf compatability

#ifndef ID_VL_H
#define ID_VL_H

#include "wl_def.h"

void Quit(const char *error, ...);

//===========================================================================

#define CHARWIDTH 2
#define TILEWIDTH 4

//===========================================================================

extern SDL_Surface  *screen, *screenBuffer, *curSurface;
extern SDL_Window   *window;
extern SDL_Renderer *renderer;

extern boolean fullscreen, usedoublebuffering;
// extern uint24_t screenWidth, screenHeight;
#define screenWidth  256
#define screenHeight 212

extern unsigned screenBits, screenPitch, bufferPitch, curPitch;
// extern unsigned scaleFactor;

#define scaleFactor 1

extern boolean  screenfaded;
extern unsigned bordercolor;

extern SDL_Color gamepal[256];

//===========================================================================

//
// VGA hardware routines
//

#define VL_WaitVBL(a) SDL_Delay((a)*8)

void VL_SetVGAPlaneMode(void);
void VL_SetTextMode(void);
void VL_Shutdown(void);

void VL_ConvertPalette(byte *srcpal, SDL_Color *destpal, int numColors);
void VL_FillPalette(int red, int green, int blue);
void VL_SetColor(int color, int red, int green, int blue);
void VL_GetColor(int color, int *red, int *green, int *blue);
void VL_SetPalette(SDL_Color *palette, bool forceupdate);
void VL_GetPalette(SDL_Color *palette);
void VL_FadeOut(int start, int end, int red, int green, int blue, int steps);
void VL_FadeIn(int start, int end, SDL_Color *palette, int steps);

byte *VL_LockSurface(SDL_Surface *surface);
void  VL_UnlockSurface(SDL_Surface *surface);

#define LOCK()   VL_LockSurface(curSurface)
#define UNLOCK() VL_UnlockSurface(curSurface)

byte VL_GetPixel(int x, int y);
void VL_Plot(int x, int y, int color);
void VL_Hlin(unsigned x, unsigned y, unsigned width, int color);
void VL_Vlin(int x, int y, int height, int color);
void VL_BarScaledCoord(int scx, int scy, int scwidth, int scheight, int color);
static void inline VL_Bar(int x, int y, int width, int height, int color) {
  VL_BarScaledCoord(scaleFactor * x, scaleFactor * y, scaleFactor * width, scaleFactor * height, color);
}
static void inline VL_ClearScreen(int color) { SDL_FillRect(curSurface, NULL, color); }

void VL_MungePic(byte *source, unsigned width, unsigned height);
void VL_DrawPicBare(int x, int y, byte *pic, int width, int height);
void VL_MemToLatch(byte *source, int width, int height, SDL_Surface *destSurface, int x, int y);
void VL_ScreenToScreen(SDL_Surface *source, SDL_Surface *dest);
void VL_MemToScreenScaledCoordA(byte *source, int width, int height, int scx, int scy);
void VL_MemToScreenScaledCoordN(
    byte *source, int origwidth, int origheight, int srcx, int srcy, int destx, int desty, int width, int height);

void inline VL_MemToScreen(byte *source, int width, int height, int x, int y) {
  VL_MemToScreenScaledCoordA(source, width, height, scaleFactor * x, scaleFactor * y);
}

void VL_MaskedToScreen(byte *source, int width, int height, int x, int y);

void VL_LatchToScreenScaledCoord7(SDL_Surface *source, int xsrc, int ysrc, int width, int height, int scxdest, int scydest);

static void inline VL_LatchToScreen(SDL_Surface *source, int xsrc, int ysrc, int width, int height, int xdest, int ydest) {
  VL_LatchToScreenScaledCoord7(source, xsrc, ysrc, width, height, scaleFactor * xdest, scaleFactor * ydest);
}
static void inline VL_LatchToScreenScaledCoord3(SDL_Surface *source, int scx, int scy) {
  VL_LatchToScreenScaledCoord7(source, 0, 0, source->w, source->h, scx, scy);
}

static void inline VL_LatchToScreenA(SDL_Surface *source, int x, int y) {
  VL_LatchToScreenScaledCoord7(source, 0, 0, source->w, source->h, scaleFactor * x, scaleFactor * y);
}

// void VL_MemToScreenScaledCoord(byte* source, int width, int height, int scx, int scy);
// void VL_MemToScreenScaledCoordA(byte* source, int origwidth, int origheight, int srcx, int srcy,int destx, int desty, int width,
// int height);

#endif
