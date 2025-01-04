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
extern byte *vbuf;

extern byte   *postsource;
extern int16_t postx;

extern void     scale_post_calc_ycount();
int16_t ywcount __data_on_chip, yoffs __data_on_chip, yw __data_on_chip, yd __data_on_chip, yendoffs __data_on_chip;

void __func_on_chip ScalePost() {
  uint8_t grb;

  scale_post_calc_ycount();

  yoffs = ((view_height >> 1) - ywcount) * view_width;
  if (yoffs < 0)
    yoffs = 0;
  yoffs += (int16_t)postx;

  yendoffs = view_height / 2 + ywcount - 1;
  yw       = TEXTURESIZE - 1;

  while (yendoffs >= view_height) {
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

  grb = postsource[yw];

  yendoffs = yendoffs * view_width + postx;
  while (yoffs <= yendoffs) {
    vbuf[yendoffs] = grb;
    ywcount -= TEXTURESIZE / 2;
    if (ywcount <= 0) {
      do {
        ywcount += yd;
        yw--;
      } while (ywcount <= 0);
      if (yw < 0)
        break;
      grb = postsource[yw];
    }
    yendoffs -= view_width;
  }
}
