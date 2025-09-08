#include <ez80.h>
#include <stdbool.h>
#include <stdlib.h>
#include <v99x8.h>

void vdp_set_refresh(const uint8_t refresh_rate) {
  if (refresh_rate == PAL) {
    registers_mirror[9] |= 0x02;
    return;
  }

  if (refresh_rate == NTSC)
    registers_mirror[9] &= ~0x02;
}
