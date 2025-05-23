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

extern uint8_t scale_post_asm();

void __func_on_chip ScalePost() {
  uint8_t grb;

  grb = scale_post_asm();

  if (drawing_params.yw < 0)
    return;

  while (drawing_params.yoffs <= drawing_params.yendoffs) {
    view_port_buffer[drawing_params.yendoffs] = grb;

    drawing_params.ywcount -= TEXTURESIZE / 2;
    if (drawing_params.ywcount <= 0) {
      do {
        drawing_params.ywcount += drawing_params.yd;
        drawing_params.yw--;
      } while (drawing_params.ywcount <= 0);
      if (drawing_params.yw < 0)
        break;
      grb = drawing_params.postsource[drawing_params.yw];
    }
    drawing_params.yendoffs -= drawing_params.view_width;
  }
}
