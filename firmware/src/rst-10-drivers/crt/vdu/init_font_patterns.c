#include "../../../q3vm/bytecode.h"
#include "../../../q3vm/vm-functions.h"
#include "../vdu.h"
#include <v99x8-super.h>

uint8_t cursor_pattern[8] = {255, 255, 255, 255, 255, 255, 255, 255};

void init_font_patterns(void) {
  uint24_t i;

  vm_vdp_set_super_graphic_1(); // can only access higher memory directly, when super mode is on

  vdp_cpu_to_vram(sysfont, FONT_8X8_STORED_Y + ((uint24_t)' ' * 8), sizeof(sysfont));

  for (i = FONT_8X8_STORED_Y + ((uint24_t)' ' * 8) + sizeof(sysfont); i < VRAM_SIZE; i += 8)
    vdp_cpu_to_vram(cursor_pattern, i, 8);

  vdp_cpu_to_vram(cursor_pattern, FONT_8X8_STORED_Y - 8, 8);
}
