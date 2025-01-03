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
extern unsigned vbufPitch;
extern byte    *vbuf;

extern byte   *postsource;
extern int16_t postx;

extern uint16_t scale_post_calc_ycount();
int ywcount __data_on_chip, yoffs __data_on_chip, yw __data_on_chip, yd __data_on_chip, yendoffs __data_on_chip;

void __func_on_chip ScalePost() {
  byte col;

  ywcount = yd = scale_post_calc_ycount();

  yoffs = (viewheight / 2 - ywcount) * vbufPitch;
  if (yoffs < 0)
    yoffs = 0;
  yoffs += (int)postx;

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

  yendoffs = yendoffs * vbufPitch + (int)postx;
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
