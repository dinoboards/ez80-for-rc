#include <sys/types.h>
#include <unistd.h>

#include "wl_def.h"

#include "id_mm.h"

#include "id_ca.h"

extern huffnode grhuffman[255];

/*
============================================================================

                                COMPRESSION routines, see JHUFF.C for more

============================================================================
*/

void CAL_HuffExpand(byte *source, byte *dest, int24_t length /*, huffnode *hufftable*/) {
  byte     *end;
  huffnode *huffptr;

  if (!length || !dest) {
    Quit("length or dest is null!");
    return;
  }

  end = dest + length;

  byte val  = *source++;
  byte mask = 1;
  word nodeval;
  huffptr = grhuffman + 254;
  while (1) {
    nodeval = (val & mask) ? huffptr->bit1 : huffptr->bit0;

    if (mask == 0x80) {
      val  = *source++;
      mask = 1;
    } else
      mask <<= 1;

    if (nodeval < 256) {
      *dest++ = (byte)nodeval;
      huffptr = grhuffman + 254;
      if (dest >= end)
        break;
    } else {
      huffptr = grhuffman + (nodeval - 256);
    }
  }
}
