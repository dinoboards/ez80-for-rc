#ifndef I2C_H
#define I2C_H

#include <stdint.h>

extern void i2c_init(const uint8_t t) __z88dk_fastcall;
extern uint8_t i2c_write(const uint8_t addr, const uint8_t len,
                         const uint8_t *data);
extern uint8_t i2c_probe(const uint8_t addr) __z88dk_fastcall;

#endif
