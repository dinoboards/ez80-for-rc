#include "../vdu.h"
#include <stdint.h>
#include <v99x8.h>

void vdu_cr(void) { current_tpos.x = tviewport.left; }
