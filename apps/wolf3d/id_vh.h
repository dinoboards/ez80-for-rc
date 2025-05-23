#ifndef __ID_VH_H__
#define __ID_VH_H__
// ID_VH.H

#include "id_vl.h"

#define WHITE         15 // graphics mode independent colors
#define BLACK         0
#define FIRSTCOLOR    1
#define SECONDCOLOR   12
#define F_WHITE       15
#define F_BLACK       0
#define F_FIRSTCOLOR  1
#define F_SECONDCOLOR 12

//===========================================================================

#define MAXSHIFTS 1

typedef struct {
  int16_t width, height;
} pictabletype;

typedef struct {
  int16_t height;
  int16_t location[256];
  int8_t  width[256];
} fontstruct;

//===========================================================================

extern pictabletype *pictable;
extern pictabletype *picmtable;

extern byte fontcolor, backcolor;
extern int  fontnumber;
extern int  px, py;

#define SETFONTCOLOR(f, b)                                                                                                         \
  fontcolor = f;                                                                                                                   \
  backcolor = b;

//
// mode independant routines
// coordinates in pixels, rounded to best screen res
// regions marked in double buffer
//

void VWB_DrawPropString(const char *string);

void VWB_DrawTile8(int x, int y, int tile);
void VWB_DrawTile16(int x, int y, int tile);
void VWB_DrawTile16M(int x, int y, int tile);
void VWB_DrawPic(int x, int y, int chunknum);
void VWB_DrawMPic(int x, int y, int chunknum);
void VWB_Plot(int x, int y, int color);
#define VWB_PlotScaledCoord VW_Plot
void VWB_Hlin(int x1, int x2, int y, int color);
void VWB_Vlin(int y1, int y2, int x, int color);

#define VH_UpdateScreen()

#define VW_UpdateScreen() VH_UpdateScreen()

//
// wolfenstein EGA compatability stuff
//

#define VW_Shutdown         VL_Shutdown
#define VW_Bar              VL_Bar
#define VW_Hlin(x, z, y, c) VL_Hlin(x, y, (z) - (x) + 1, c)
#define VW_Vlin(y, z, x, c) VL_Vlin(x, y, (z) - (y) + 1, c)
#define VW_DrawPic          VH_DrawPic
#define VW_WaitVBL          VL_WaitVBL
#define VW_FadeIn()         VL_FadeIn(0, 255, gamepal, 30);
#define VW_FadeOut()        VL_FadeOut(0, 255, 0, 0, 0, 30);
void VW_MeasurePropString(const char *string, word *width, word *height);

#ifdef APP_DEBUG
#define LatchDrawChar(x, y, p) VL_LatchToScreen(latchpics[0], ((p)&7) * 8, ((p) >> 3) * 8 * 64, 8, 8, x, y)
#endif

void LatchDrawPic(unsigned x, unsigned y, unsigned picnum);
void DrawLatchToSurface(unsigned x, unsigned y, unsigned picnum);
void LoadLatchMem(void);

void VH_Startup();
// boolean FizzleFade(SDL_Surface *source, int x1, int y1, unsigned width, unsigned height, unsigned frames, boolean abortable);
#define FizzleFade(...)

#define NUMLATCHPICS 100
extern pixel_surface_t *latchpics[NUMLATCHPICS];

static inline void VWB_Bar(int x, int y, int width, int height, int color) { VW_Bar(x, y, width, height, color); }

#endif
