#ifndef I2C_H
#define I2C_H

#include <stdint.h>

#define I2C_BUS_FREQ_100KHZ 0
#define I2C_BUS_FREQ_400KHZ 1

typedef struct
{
  uint8_t scalar;   // I2C clock divider scalar value
  uint8_t exponent; // I2C clock divider exponent
  uint32_t bus_frequency;
} i2c_clk_divider;

extern uint8_t i2c_init(const uint8_t t, i2c_clk_divider *) __sdcccall(1);
extern uint8_t i2c_write(const uint8_t addr, const uint8_t len,
                         const uint8_t *data);
extern uint8_t i2c_probe(const uint8_t addr) __z88dk_fastcall;

#endif
