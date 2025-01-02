#include <ez80.h>
// WL_DRAW.C

#include "wl_def.h"

#include "id_vh.h"

//==========================================================================

/*
===================
=
= ScalePost
=
===================
*/
extern unsigned     vbufPitch;
extern  byte *vbuf;


extern byte *postsource;
extern int   postx;

void __func_on_chip ScalePost() {
  int  ywcount, yoffs, yw, yd, yendoffs;
  byte col;

  ywcount = yd = wallheight[postx] >> 3;
  if (yd <= 0)
    yd = 100;

  yoffs = (viewheight / 2 - ywcount) * vbufPitch;
  if (yoffs < 0)
    yoffs = 0;
  yoffs += postx;

  yendoffs = viewheight / 2 + ywcount - 1;
  yw       = TEXTURESIZE - 1;

  while (yendoffs >= viewheight) {
    ywcount -= TEXTURESIZE / 2;
    while (ywcount <= 0) {
      ywcount += yd;
      yw--;
    }
    yendoffs--;
  }
  if (yw < 0) {
    return;
  }

  col = postsource[yw];

  yendoffs = yendoffs * vbufPitch + postx;
  while (yoffs <= yendoffs) {
    vbuf[yendoffs] = col;
    ywcount -= TEXTURESIZE / 2;
    if (ywcount <= 0) {
      do {
        ywcount += yd;
        yw--;
      } while (ywcount <= 0);
      if (yw < 0)
        break;
      col = postsource[yw];
    }
    yendoffs -= vbufPitch;
  }
}
