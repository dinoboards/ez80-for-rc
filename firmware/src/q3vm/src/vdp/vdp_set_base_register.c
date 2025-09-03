#include <ez80.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <v99x8.h>

void set_base_registers(void) {
#pragma asm DI

  uint8_t *pReg = registers_mirror;
  uint8_t  i;

  for (i = 0; i < REGISTER_COUNT; i++) {
    vdp_reg_write(i, *pReg); // if we inline the increment, the compiler (with -Oz seems to pre-increment the pointer)

    VDP_ADDR = *pReg;
    VDP_ADDR = i | 0x80;

    pReg++;
  }

#pragma asm EI
}
