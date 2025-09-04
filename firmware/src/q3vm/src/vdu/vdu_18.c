#include <vm-shared-mem.h>

// VDU: 18 (2 bytes)
void vdu_gcol(void) {
  current_operation_mode = data[0];
  current_gfg_colour     = data[1];
}
