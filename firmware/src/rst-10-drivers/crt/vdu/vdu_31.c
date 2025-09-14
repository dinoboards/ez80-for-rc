#include "../vdu.h"

void vdu_tab(void) {
  current_tpos.x = (int24_t)data[0] * 8;
  current_tpos.y = (int24_t)data[1] * 8;
}
