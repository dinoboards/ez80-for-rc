#include <stdint.h>
#include <stdlib.h>

extern unsigned long cpu_freq_calculated;

uint8_t i2c_calc_ccr(const uint8_t t)
{
  const uint24_t target = t == 0 ? 10000 : 40000;
  uint8_t m_result = 0;
  uint8_t n_result = 0;

  uint8_t m, n;
  uint24_t y;
  uint8_t z;
  uint24_t bus_freq;
  uint24_t diff;

  uint24_t closest_freq_diff = 1000000;
  uint24_t closest_freq;

  for (n = 0; n <= 7; n++)
  {
    for (m = 1; m <= 16; m++)
    {
      z = 1 << n;
      y = (100 * m * z);
      bus_freq = cpu_freq_calculated / y;

      if (bus_freq == target)
      {
        m_result = m;
        n_result = n;
        goto done;
      }

      diff = abs(bus_freq - target);

      if (diff < closest_freq_diff && bus_freq < (target + 100))
      {
        closest_freq_diff = diff;
        closest_freq = bus_freq;
        m_result = m;
        n_result = n;
      }
    }
  }

done:
  return ((m_result - 1) << 3) | n_result;
}
