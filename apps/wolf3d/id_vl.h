// ID_VL.H

// wolf compatibility

void VL_Bar(int scx, int scy, int scwidth, int scheight, uint8_t color);

#ifndef ID_VL_H
#define ID_VL_H

#include "ez80-vdp.h"
#include "wl_def.h"

void Quit(const char *error, ...);

//===========================================================================

#define CHARWIDTH 2
#define TILEWIDTH 4

//===========================================================================

extern SDL_Window   *window;
extern SDL_Renderer *renderer;

extern boolean fullscreen, usedoublebuffering;
#define screenWidth  MAXVIEWWIDTH
#define screenHeight SCREEN_HEIGHT

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

void VL_SetTextMode(void);

void VL_ConvertPalette(byte *srcpal, SDL_Color *destpal, int numColors);
void VL_FillPalette(int red, int green, int blue);
void VL_SetColor(int color, int red, int green, int blue);
void VL_GetColor(int color, int *red, int *green, int *blue);
void VL_SetPalette(SDL_Color *palette, bool forceupdate);
void VL_GetPalette(SDL_Color *palette);
void VL_FadeOut(int start, int end, int red, int green, int blue, int steps);
void VL_FadeIn(int start, int end, SDL_Color *palette, int steps);

void VL_Hlin(unsigned x, unsigned y, unsigned width, uint8_t color);
void VL_Vlin(int x, int y, int height, int color);
static void inline VL_ClearScreen(int color) { vdp_scn_clear(color); }

void VL_MemToLatch(byte *source, int width, int height, pixel_surface_t *destSurface, int x, int y);
void VL_MemToScreen(const byte *source, int width, int height, int scx, int scy);

void VL_MaskedToScreen(byte *source, int width, int height, int x, int y);

void VL_LatchToScreen(pixel_surface_t *source, int xsrc, int ysrc, int width, int height, int scxdest, int scydest);
void VL_SurfaceToScreen(pixel_surface_t *source, int scxdest, int scydest);

/*
=======================
=
= VL_InitVideoMode
=
=======================
*/
static inline void VL_InitVideoMode(void) { vdp_scn_init(); }

static inline void VL_Shutdown(void) {}

#endif
