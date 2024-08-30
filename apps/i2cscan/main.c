#include "i2c.h"
#include <stdint.h>
#include <stdio.h>

uint8_t i;
uint8_t stat = 0;

void main(void) {

  i2c_init(0);

  i = 0x70;
  stat = i2c_probe(i);
  printf("scan of : %d = %02X\r\n", i, stat);

  for (i = 1; i < 128; i++) {
    stat = i2c_probe(i);
    printf("scan of : %d = %02X\r\n", i, stat);
  }
}
