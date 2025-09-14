#include "vdu.h"
#include <stdint.h>

#include <stdio.h>

void vdu_init(void) {
  init_font_patterns();

  gviewport.left   = 0;
  gviewport.bottom = 0;
  gviewport.right  = scale_width - 1;
  gviewport.top    = scale_height - 1;
}
