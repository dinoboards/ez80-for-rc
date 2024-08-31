#include "i2c.h"
#include <stdint.h>
#include <stdio.h>

uint8_t i;
uint8_t stat = 0;

i2c_clk_divider i2c_clk_divider;

void main(void)
{
  stat = i2c_init(I2C_BUS_FREQ_100KHZ, &i2c_clk_divider);
  printf("I2C Init: STAT: %02X; Actual Frequency: %ld, scalar: %d, exponent: %d\r\n", stat, i2c_clk_divider.bus_frequency, i2c_clk_divider.scalar, i2c_clk_divider.exponent);

  i = 0x70;
  stat = i2c_probe(i);
  printf("scan of : %d = %02X\r\n", i, stat);

  for (i = 1; i < 128; i++)
  {
    stat = i2c_probe(i);
    printf("scan of : %d = %02X\r\n", i, stat);
  }
}
