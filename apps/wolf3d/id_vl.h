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

extern SDL_Surface  *screenBuffer;
extern SDL_Window   *window;
extern SDL_Renderer *renderer;

extern boolean fullscreen, usedoublebuffering;
#define screenWidth  MAXVIEWWIDTH
#define screenHeight 212

extern unsigned screenBits, screenPitch;

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

void VL_Plot(int x, int y, int color);
void VL_Hlin(unsigned x, unsigned y, unsigned width, int color);
void VL_Vlin(int x, int y, int height, int color);
void VL_BarScaledCoord(int scx, int scy, int scwidth, int scheight, int color);
static void inline VL_Bar(int x, int y, int width, int height, int color) { VL_BarScaledCoord(x, y, width, height, color); }
static void inline VL_ClearScreen(int color) { SDL_FillRect(screenBuffer, NULL, color); }

void VL_MungePic(byte *source, unsigned width, unsigned height);
void VL_DrawPicBare(int x, int y, byte *pic, int width, int height);
void VL_MemToLatch(byte *source, int width, int height, SDL_Surface *destSurface, int x, int y);
void VL_ScreenToScreen(SDL_Surface *source, SDL_Surface *dest);
void VL_MemToScreenScaledCoordA(byte *source, int width, int height, int scx, int scy);
void VL_MemToScreenScaledCoordN(
    byte *source, int origwidth, int origheight, int srcx, int srcy, int destx, int desty, int width, int height);

static void inline VL_MemToScreen(byte *source, int width, int height, int x, int y) {
  VL_MemToScreenScaledCoordA(source, width, height, x, y);
}

void VL_MaskedToScreen(byte *source, int width, int height, int x, int y);

void VL_LatchToScreen(SDL_Surface *source, int xsrc, int ysrc, int width, int height, int scxdest, int scydest);

static void inline VL_SurfaceToScreen(SDL_Surface *source, int x, int y) {
  VL_LatchToScreen(source, 0, 0, source->w, source->h, x, y);
}

#endif
