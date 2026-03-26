#include "../rst-28-vars.h"
#include <v99x8-super.h>

/*
VDU 17,n
VDU 17 sets either the text foreground (n<128) or background (n>=128) colours to
the value n. It is equivalent to COLOUR n
*/
void vdu_colour(void) {
  vdu_vars_t *const vdu = &hbios_vars->vdu;

  if (vdu->data[0] >= 128)
    vdu->current_tbg_colour = vdu->data[0] & vdu->current_mode_colour_mask;
  else
    vdu->current_tfg_colour = vdu->data[0] & vdu->current_mode_colour_mask;

  vdp_set_remap(vdu->current_tbg_colour, vdu->current_tfg_colour);
}
