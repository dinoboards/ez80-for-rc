#include "wl_draw.h"
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

extern byte *postsource;

extern void scale_post_calc_ycount();

void __func_on_chip ScalePost() {
  uint8_t grb;

  scale_post_calc_ycount();

  drawing_params.yendoffs = drawing_params.view_half_height + drawing_params.ywcount - 1;
  drawing_params.yw       = TEXTURESIZE - 1;

  while (drawing_params.yendoffs >= drawing_params.view_height) {
    drawing_params.ywcount -= TEXTURESIZE / 2;
    while (drawing_params.ywcount <= 0) {
      drawing_params.ywcount += drawing_params.yd;
      drawing_params.yw--;
    }
    drawing_params.yendoffs--;
  }
  if (drawing_params.yw < 0) {
    return;
  }

  grb = postsource[drawing_params.yw];

  drawing_params.yendoffs = drawing_params.yendoffs * drawing_params.view_width + drawing_params.postx;
  while (drawing_params.yoffs <= drawing_params.yendoffs) {
    vbuf[drawing_params.yendoffs] = grb;
    drawing_params.ywcount -= TEXTURESIZE / 2;
    if (drawing_params.ywcount <= 0) {
      do {
        drawing_params.ywcount += drawing_params.yd;
        drawing_params.yw--;
      } while (drawing_params.ywcount <= 0);
      if (drawing_params.yw < 0)
        break;
      grb = postsource[drawing_params.yw];
    }
    drawing_params.yendoffs -= drawing_params.view_width;
  }
}
