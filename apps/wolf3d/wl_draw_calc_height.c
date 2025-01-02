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

extern int32_t xintercept, yintercept;
extern int     min_wallheight;

int __func_on_chip CalcHeight() {
  fixed z = FixedMul(xintercept - viewx, viewcos) - FixedMul(yintercept - viewy, viewsin);

  if (z < MINDIST)
    z = MINDIST;

  int height = heightnumerator / (z >> 8);

  if (height < min_wallheight)
    min_wallheight = height;

  return height;
}
