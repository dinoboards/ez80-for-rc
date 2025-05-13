#include "wl_draw.h"

#include "wl_def.h"

#include "id_mm.h"

#include "ez80-vdp.h"
#include "v99x8-hdmi/v99x8-wolf3d.h"

pictabletype *pictable; // TODO this can be pre-allocated based on NUMPICS

pixel_surface_t *latchpics[NUMLATCHPICS];

int  px, py;
byte fontcolor, backcolor;
int  fontnumber;

//==========================================================================

void VWB_DrawPropString(const char *string) {
  fontstruct *font;
  int         width, step, height;
  byte       *source;
  byte        ch;

  font   = (fontstruct *)grsegs[STARTFONT + fontnumber];
  height = font->height;

  uint16_t x = px;

  while ((ch = (byte)*string++) != 0) {
    width = step = font->width[ch];
    source       = ((byte *)font) + font->location[ch];

    vdp_scn_font(source, x, py, width, height, fontcolor);
    x += width;
  }
}

void VWL_MeasureString(const char *string, word *width, word *height, fontstruct *font) {
  *height = font->height;
  for (*width = 0; *string; string++)
    *width += font->width[*((byte *)string)]; // proportional width
}

void VW_MeasurePropString(const char *string, word *width, word *height) {
  VWL_MeasureString(string, width, height, (fontstruct *)grsegs[STARTFONT + fontnumber]);
}

/*
=============================================================================

                                Double buffer management routines

=============================================================================
*/

#ifdef APP_DEBUG
void VWB_DrawTile8(int x, int y, int tile) { LatchDrawChar(x, y, tile); }
#endif

void VWB_DrawPic(int x, int y, int chunknum) {
  int      picnum = chunknum - STARTPICS;
  unsigned width, height;

  width  = pictable[picnum].width;
  height = pictable[picnum].height;

  // switch (chunknum) {
  // case 11:
  // case 12:
  //   break;

  // default:
  // printf("VMB_DrawPic(%d, %d, %d) {%d, %d}\r\n", x, y, chunknum, width, height);
  // }

  VL_MemToScreen(grsegs[chunknum], width, height, x, y);
}

void spike_draw_status_bar_pic_adjusted() {
  CA_CacheGrChunk(STATUSBARPIC);
  const uint8_t *source = grsegs[STATUSBARPIC];

  vdp_scn_write_init();

  for (uint16_t y = 0; y < STATUSLINES; y++) {
    uint16_t xx = STATUSBARPIC_LEFT_OFFSET;
    for (uint16_t x = 0; x < SCREEN_WIDTH; x++) {
      const uint8_t color = gamepal[source[(y * (320 >> 2) + (xx >> 2)) + (xx & 3) * (320 >> 2) * STATUSLINES]];

      vdp_scn_write_pixel(color, 0, SCREEN_HEIGHT - STATUSLINES, SCREEN_WIDTH, STATUSLINES);
      xx++;
    }
  }
}

// void VWB_Plot(int x, int y, int color) {
//   VW_Plot(x, y, color);
// }

void VWB_Hlin(int x1, int x2, int y, int color) { VW_Hlin(x1, x2, y, color); }

void VWB_Vlin(int y1, int y2, int x, int color) { VW_Vlin(y1, y2, x, color); }

/*
=============================================================================

                                                WOLFENSTEIN STUFF

=============================================================================
*/

/*
=====================
=
= LatchDrawPic
=
=====================
*/

void LatchDrawPic(unsigned x, unsigned y, unsigned picnum) {
  // printf("LatchDrawPic(%d, %d, %d)\r\n", x, y, picnum);
  pixel_surface_t *source = latchpics[2 + picnum - LATCHPICS_LUMP_START];
  VL_SurfaceToScreen(source, x, y);
}

//==========================================================================

/*
===================
=
= LoadLatchMem
=
===================
*/

void LoadLatchMem(void) {
  int   start, end;
  byte *src;
  //
  // tile 8s
  //

  const uint16_t width  = 8 * 8;
  const uint16_t height = ((NUMTILE8 + 7) / 8) * 8;
  MM_GetPtr((memptr *)&latchpics[0], sizeof(pixel_surface_t) + width * height);
  create_pixel_surface(latchpics[0], width, height);
  if (latchpics[0] == NULL) {
    Quit("Unable to create surface for tiles!");
  }

  CA_CacheGrChunk(STARTTILE8);
  src = grsegs[STARTTILE8];

  for (int i = 0; i < NUMTILE8; i++) {
    VL_MemToLatch(src, 8, 8, latchpics[0], (i & 7) * 8, (i >> 3) * 8);
    src += 64;
  }
  UNCACHEGRCHUNK(STARTTILE8);

  //
  // pics
  //
  start = LATCHPICS_LUMP_START;
  end   = LATCHPICS_LUMP_END;

  for (int i = start; i <= end; i++) {
    const uint16_t width  = pictable[i - STARTPICS].width;
    const uint16_t height = pictable[i - STARTPICS].height;

    MM_GetPtr((memptr *)&latchpics[2 + i - start], sizeof(pixel_surface_t) + width * height);
    create_pixel_surface(latchpics[2 + i - start], width, height);
    if (latchpics[2 + i - start] == NULL) {
      Quit("Unable to create surface for picture!");
    }

    CA_CacheGrChunk(i);
    VL_MemToLatch(grsegs[i], width, height, latchpics[2 + i - start], 0, 0);
    UNCACHEGRCHUNK(i);
  }
}

//==========================================================================

/*
===================
=
= FizzleFade
=
= returns true if aborted
=
= It uses maximum-length Linear Feedback Shift Registers (LFSR) counters.
= You can find a list of them with lengths from 3 to 168 at:
= http://www.xilinx.com/support/documentation/application_notes/xapp052.pdf
= Many thanks to Xilinx for this list!!!
=
===================
*/

// XOR masks for the pseudo-random number sequence starting with n=17 bits
static const uint32_t rndmasks[] = {
    // n    XNOR from (starting at 1, not 0 as usual)
    0x00012000, // 17   17,14
    0x00020400, // 18   18,11
    0x00040023, // 19   19,6,2,1
    0x00090000, // 20   20,17
    0x00140000, // 21   21,19
    0x00300000, // 22   22,21
    0x00420000, // 23   23,18
    0x00e10000, // 24   24,23,22,17
    0x01200000, // 25   25,22      (this is enough for 8191x4095)
};

static unsigned int rndbits_y;
static unsigned int rndmask;

// Returns the number of bits needed to represent the given value
static int log2_ceil(uint32_t x) {
  int      n = 0;
  uint32_t v = 1;
  while (v < x) {
    n++;
    v <<= 1;
  }
  return n;
}

void VH_Startup() {
  int rndbits_x = log2_ceil(screenWidth);
  rndbits_y     = log2_ceil(screenHeight);

  int rndbits = rndbits_x + rndbits_y;
  if (rndbits < 17)
    rndbits = 17; // no problem, just a bit slower
  else if (rndbits > 25)
    rndbits = 25; // fizzle fade will not fill whole screen

  rndmask = rndmasks[rndbits - 17];
}

// boolean FizzleFade(SDL_Surface *source, int x1, int y1, unsigned width, unsigned height, unsigned frames, boolean abortable) {
//   unsigned x, y, frame, pixperframe;
//   int32_t  rndval, lastrndval;
//   int      first = 1;

//   lastrndval  = 0;
//   pixperframe = width * height / frames;

//   IN_StartAck();

//   frame = GetTimeCount();

//   // can't rely on screen as dest b/c crt.cpp writes over it with screenBuffer
//   // can't rely on screenBuffer as source for same reason: every flip it has to be updated
//   SDL_Surface *source_copy = SDL_ConvertSurface(source, source->format, source->flags);
//   SDL_Surface *screen_copy = SDL_ConvertSurface(screen, screen->format, screen->flags);

//   byte *srcptr = source_copy->xpixels;
//   do {
//     if (abortable && IN_CheckAck()) {
//       SDL_BlitSurface(screen_copy, NULL, screenBuffer, NULL);
//       SDL_BlitSurface(screenBuffer, NULL, screen, NULL);
//       SDL_Flip(screen);
//       SDL_FreeSurface(source_copy);
//       SDL_FreeSurface(screen_copy);
//       return true;
//     }

//     byte *destptr = screen_copy->xpixels;

//     rndval = lastrndval;

//     // When using double buffering, we have to copy the pixels of the last AND the current frame.
//     // Only for the first frame, there is no "last frame"
//     for (int i = first; i < 2; i++) {
//       for (unsigned p = 0; p < pixperframe; p++) {
//         //
//         // seperate random value into x/y pair
//         //

//         x = rndval >> rndbits_y;
//         y = rndval & ((1 << rndbits_y) - 1);

//         //
//         // advance to next random element
//         //

//         rndval = (rndval >> 1) ^ (rndval & 1 ? 0 : rndmask);

//         if (x >= width || y >= height) {
//           if (rndval == 0) // entire sequence has been completed
//             goto finished;
//           p--;
//           continue;
//         }

//         //
//         // copy one pixel
//         //

//         if (screenBits == 8) {
//           *(destptr + (y1 + y) * screen->pitch + x1 + x) = *(srcptr + (y1 + y) * source->pitch + x1 + x);
//         } else {
//           byte    col     = *(srcptr + (y1 + y) * source->pitch + x1 + x);
//           uint8_t fullcol = curpal[col]; // SDL_MapRGB(/*screen->format,*/ curpal[col].r, curpal[col].g, curpal[col].b);
//           memcpy(destptr + (y1 + y) * screen->pitch + (x1 + x) /* screen->format->BytesPerPixel*/, &fullcol,
//                  1 /*screen->format->BytesPerPixel*/);
//         }

//         if (rndval == 0) // entire sequence has been completed
//           goto finished;
//       }

//       if (!i || first)
//         lastrndval = rndval;
//     }

//     // If there is no double buffering, we always use the "first frame" case
//     if (usedoublebuffering)
//       first = 0;

//     SDL_BlitSurface(screen_copy, NULL, screenBuffer, NULL);
//     SDL_BlitSurface(screenBuffer, NULL, screen, NULL);
//     SDL_Flip(screen);

//     frame++;
//     Delay(frame - GetTimeCount()); // don't go too fast
//   } while (1);

// finished:
//   SDL_BlitSurface(screen_copy, NULL, screenBuffer, NULL);
//   SDL_BlitSurface(screenBuffer, NULL, screen, NULL);
//   SDL_Flip(screen);
//   SDL_FreeSurface(source_copy);
//   SDL_FreeSurface(screen_copy);
//   return false;
// }
