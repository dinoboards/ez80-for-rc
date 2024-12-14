#include <stdint.h>
#include <v99x8.h>

void vdp_cpu_to_vram(const uint8_t *const source, uint24_t vdp_address, uint16_t length) {

  const uint8_t bits_16_14 = (vdp_address >> 14) & 0x07;
  const uint8_t bits_13_8  = (vdp_address >> 8) & 0x3F;
  const uint8_t bits_7_0   = vdp_address & 0xFF;

  vdp_reg_write(14, bits_16_14); // bit16..14 (extended has more bits)
  vdp_out_cmd(bits_7_0);         // bits 7..0
  vdp_out_cmd(0x40 | bits_13_8); // 0x40 | bits 13..8

  const uint8_t *c = source;

  for (int y = 0; y < length; y++) {
    VDP_DATA = *c++;
  }
}
