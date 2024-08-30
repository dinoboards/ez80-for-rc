#ifndef ADAFRUIT_8X8MATRIX_H
#define ADAFRUIT_8X8MATRIX_H

#include <stdint.h>

#define LED_ON 1  // GFX color of lit LED segments (single-color displays)
#define LED_OFF 0 // GFX color of unlit LED segments (single-color displays)

#define HT16K33_BLINK_CMD 0x80       // I2C register for BLINK setting
#define HT16K33_BLINK_DISPLAYON 0x01 // I2C value for steady on
#define HT16K33_BLINK_OFF 0          // I2C value for steady off
#define HT16K33_BLINK_2HZ 1          // I2C value for 2 Hz blink
#define HT16K33_BLINK_1HZ 2          // I2C value for 1 Hz blink
#define HT16K33_BLINK_HALFHZ 3       // I2C value for 0.5 Hz blink

#define HT16K33_CMD_BRIGHTNESS 0xE0 // I2C register for BRIGHTNESS setting

void adafruit_8x8matrix_init(void);
void adafruit_8x8matrix_display(void);
void adafruit_8x8matrix_blink_rate(uint8_t b);
void adafruit_8x8matrix_set_brightness(uint8_t b);

#endif
