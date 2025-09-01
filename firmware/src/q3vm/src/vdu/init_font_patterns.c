#include <stdio.h>
#include <v99x8-super.h>

extern const uint8_t sysfont[(128 - ' ') * 8]; // 96*8

#define VRAM_SIZE         0x100000
#define FONT_8X8_STORED_Y (VRAM_SIZE - (8L * 256L))

void init_font_patterns(void) {
  vdp_set_super_graphic_1(); // can only access higher memory directly, when super mode is on

  vdp_cpu_to_vram(sysfont, FONT_8X8_STORED_Y + ((long)' ' * 8L), sizeof(sysfont));
  printf("Fonts data loaded into VRAM at %d\n", FONT_8X8_STORED_Y);
}
