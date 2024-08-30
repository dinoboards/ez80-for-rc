#include "adafruit-8x8matrix.h"
#include "i2c.h"
#include <conio.h>
#include <stdint.h>
#include <stdio.h>

#define target_address 0x70

uint8_t stat = 0;
uint8_t buffer[20];

unsigned char rotr(unsigned char c) __z88dk_fastcall {
  return (c >> 1) | (c << 7);
}

uint8_t d;
uint8_t i;

void sleep_a_bit(void);

void main(void) {
  i2c_init(0);

  adafruit_8x8matrix_init();
  if (stat != 0)
    goto error1;

  while (1) {
    if (kbhit()) {
      printf("\r\n");
      exit(0);
    }

    for (i = 0; i < 8; i++) {
      buffer[1 + (i * 2)] = rotr(d + i);
      buffer[1 + ((i + 1) * 2)] = 0xFF;
    }

    d++;

    printf("D: %02X\r", d);

    sleep_a_bit();

    adafruit_8x8matrix_display();

    if (stat != 0)
      goto error2;
  }

error1:
  printf("\r\nError1: %02X\r\n", stat);
  return;

error2:
  printf("\r\nError2: %02X\r\n", stat);
}

void sleep_a_bit(void) {
  for (uint32_t i = 0; i < 0x60000; i++)
    ;
}

void adafruit_8x8matrix_init(void) {
  buffer[0] = 0x21;
  stat = i2c_write(target_address, 1, buffer);
  if (stat != 0) {
    printf("Retrying reset... due to error %02X\r\n", stat);
    stat = i2c_write(target_address, 1, buffer);
  }

  printf("reset stat: %02X\r\n", stat);
  if (stat != 0)
    return;

  adafruit_8x8matrix_blink_rate(HT16K33_BLINK_OFF);
  if (stat != 0)
    return;

  adafruit_8x8matrix_set_brightness(15);
}

void adafruit_8x8matrix_blink_rate(uint8_t b) {
  if (b > 3)
    b = 0;

  printf("Setting Blink Rate %d ...", b);

  buffer[0] = HT16K33_BLINK_CMD | HT16K33_BLINK_DISPLAYON | (b << 1);
  stat = i2c_write(target_address, 1, buffer);
  printf(" %02X\r\n", stat);
}

void adafruit_8x8matrix_set_brightness(uint8_t b) {
  if (b > 15)
    b = 15;

  printf("Setting Brightness %d ...", b);

  buffer[0] = HT16K33_CMD_BRIGHTNESS | b;
  stat = i2c_write(target_address, 1, buffer);
  printf(" %02X\r\n", stat);
}

void adafruit_8x8matrix_display(void) {

  buffer[0] = 0;

  stat = i2c_write(target_address, 17, buffer);

  if (stat != 0) {
    printf("Retrying... due to error %02X\r\n", stat);

    stat = i2c_write(target_address, 17, buffer);
  }
}
