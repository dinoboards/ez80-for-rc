#include ".. /../../q3vm/vm-functions.h"
#include "../vdu.h"
#include <v99x8-super.h>

void init_font_patterns(void) {
  vm_vdp_set_super_graphic_1(); // can only access higher memory directly, when super mode is on

  vdp_cpu_to_vram(sysfont, FONT_8X8_STORED_Y + ((long)' ' * 8L), sizeof(sysfont));
}
