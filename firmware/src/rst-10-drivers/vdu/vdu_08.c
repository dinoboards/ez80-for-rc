#include "vdu.h"

void vdu_bs() {
  if (current_tpos.x > tviewport.left)
    current_tpos.x--;
}
