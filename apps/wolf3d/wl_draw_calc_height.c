#include <ez80.h>

#include "wl_def.h"

#include "id_vh.h"

//==========================================================================

/*
====================
=
= CalcHeight
=
= Calculates the height of xintercept,yintercept from viewx,viewy
=
====================
*/

extern fixed   xintercept, yintercept;
extern int16_t min_wallheight;

uint16_t __func_on_chip CalcHeight() {
  const fixed z = FixedMul(xintercept - viewx, viewcos) - FixedMul(yintercept - viewy, viewsin);

  const int16_t height = z < MINDIST ? heightnumerator / (MINDIST >> 8) : heightnumerator / (z >> 8);

  if (height < min_wallheight)
    min_wallheight = height;

  return height;
}
