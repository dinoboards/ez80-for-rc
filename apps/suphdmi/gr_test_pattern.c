#include "test_pattern.h"
#include "wait_for_key.h"
#include <stdio.h>
#include <v99x8.h>

extern void log_mode();

void graphics_mode_test_pattern(uint8_t mode, uint8_t refresh_rate, RGB *palette) {
  vdp_set_refresh(refresh_rate);
  for (int l = 192; l <= 212; l += 20) {
    vdp_set_lines(l);
    vdp_set_graphic_mode(mode);
    vdp_set_palette(palette);

    log_mode();

    test_pattern(8, 1);
  }
}
