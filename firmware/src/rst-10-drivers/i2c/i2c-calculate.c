#include <stdint.h>
#include <stdlib.h>

struct i2c_clk_divider
{
  uint8_t scalar;   // I2C clock divider scalar value
  uint8_t exponent; // I2C clock divider exponent
};

extern struct i2c_clk_divider i2_clk_divider;
extern uint24_t i2c_bus_freq; // I2C bus frequency
extern unsigned long cpu_freq_calculated;

uint8_t i2c_calc_ccr(const uint8_t t)
{
  uint8_t m, n;
  uint24_t y;
  uint24_t bus_freq;
  uint24_t diff;

  const uint24_t target = t == 0 ? 10000 : 40000;
  uint24_t closest_freq_diff = 1000000;

  i2_clk_divider.scalar = 255;
  i2_clk_divider.exponent = 255;
  bus_freq = 0;

  for (n = 0; n <= 7; n++)
  {
    for (m = 1; m <= 16; m++)
    {
      y = (100 * m * (1 << n));
      bus_freq = cpu_freq_calculated / y;

      if (bus_freq == target)
      {
        i2c_bus_freq = bus_freq * 10;
        i2_clk_divider.scalar = m - 1;
        i2_clk_divider.exponent = n;
        goto done;
      }

      diff = abs(bus_freq - target);

      if (diff < closest_freq_diff && bus_freq < (target + 100))
      {
        closest_freq_diff = diff;
        i2c_bus_freq = bus_freq * 10;
        i2_clk_divider.scalar = m - 1;
        i2_clk_divider.exponent = n;
      }
    }
  }

done:
  return (i2_clk_divider.scalar << 3) | i2_clk_divider.exponent;
}
