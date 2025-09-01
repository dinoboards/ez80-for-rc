#include "../vdu.h"

// VDU: 18 (2 bytes)
void vdu_gcol() {
  current_operation_mode = data[0];
  current_gfg_colour     = data[1];
}
