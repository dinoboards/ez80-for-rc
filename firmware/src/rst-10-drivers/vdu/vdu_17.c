#include "crt.h"
#include "vdu.h"

/*
VDU 17,n
VDU 17 sets either the text foreground (n<128) or background (n>=128) colours to
the value n. It is equivalent to COLOUR n
*/
void vdu_colour() {
  if (data[0] >= 128)
    current_tbg_colour = data[0] & current_mode_colour_mask;
  else
    current_tfg_colour = data[0] & current_mode_colour_mask;

  crt_font_set_colours(current_tbg_colour, current_tfg_colour);
}
