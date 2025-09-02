#include "../vm-promoted-fn.h"
#include "vdu/variables.h"
#include <host-functions.h>
#include <stdint.h>
#include <v99x8-super.h>

void graphic_print_char(uint24_t ch) {
  vdp_draw_char(ch, current_tpos.x, current_tpos.y);

  current_tpos.x += 8;
  if (current_tpos.x > tviewport.right) {
    vdu_cr();
    vdu_lf();
  }
}
