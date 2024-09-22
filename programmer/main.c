#include "pico/stdlib.h"
#include <stdio.h>
#ifdef CYW43_WL_GPIO_LED_PIN
#include "pico/cyw43_arch.h"
#endif

#define LOW  false
#define HIGH true

#define ZDI_ZCL_PIN 15
#define ZDI_ZDA_PIN 14

#define DELAY_HALF() sleep_us(1)
#define DELAY()                                                                                                                    \
  {                                                                                                                                \
    DELAY_HALF();                                                                                                                  \
    DELAY_HALF();                                                                                                                  \
  }

// #define DELAY() {for(int i = 0; i < 20; i++) { __asm volatile ("nop\n"); } }

static inline void float_pin_high(const uint8_t pin) { gpio_set_dir(pin, GPIO_IN); }

void pin_low(uint8_t pin) {
  gpio_set_dir(pin, GPIO_OUT);
  gpio_put(pin, LOW);
}

int init_zdi_pins(void) {
  gpio_set_function(ZDI_ZDA_PIN, GPIO_FUNC_NULL);
  gpio_set_function(ZDI_ZCL_PIN, GPIO_FUNC_NULL);

  gpio_pull_up(ZDI_ZDA_PIN);
  gpio_pull_up(ZDI_ZCL_PIN);

  gpio_init(ZDI_ZDA_PIN);
  gpio_init(ZDI_ZCL_PIN);

  float_pin_high(ZDI_ZDA_PIN);
  float_pin_high(ZDI_ZCL_PIN);

  return PICO_OK;
}

void start_condition() {
  float_pin_high(ZDI_ZDA_PIN);
  DELAY();

  float_pin_high(ZDI_ZCL_PIN);
  DELAY_HALF();

  pin_low(ZDI_ZDA_PIN);
  DELAY_HALF();

  pin_low(ZDI_ZCL_PIN);
  DELAY_HALF();
}

void send_bit(const bool bit) {
  if (bit)
    float_pin_high(ZDI_ZDA_PIN);
  else
    pin_low(ZDI_ZDA_PIN);

  DELAY_HALF();
  float_pin_high(ZDI_ZCL_PIN);
  DELAY();
  pin_low(ZDI_ZCL_PIN);
  DELAY_HALF();
}

bool send_address(uint8_t dat, bool read_write, bool separator) {

  start_condition();

  for (uint8_t i = 8; i; i--) {
    send_bit(dat & 0x80);
    dat <<= 1;
  }

  send_bit(read_write);
  send_bit(separator);
}

uint8_t read_bit() {
  uint8_t result = 0;

  DELAY_HALF();
  float_pin_high(ZDI_ZCL_PIN);
  DELAY_HALF();
  if (gpio_get(ZDI_ZDA_PIN))
    result = 1;
  DELAY_HALF();
  pin_low(ZDI_ZCL_PIN);
  DELAY_HALF();

  return result;
}

uint8_t read_byte() {
  uint8_t result = 0;

  float_pin_high(ZDI_ZDA_PIN);

  for (uint8_t i = 0; i < 8; i++) {
    result <<= 1;
    result |= read_bit();
  }

  return result;
}

uint8_t read_reg_byte(const uint8_t reg_addr) {
  send_address(reg_addr, 1, 0);
  return read_byte();
}

int main() {
  int rc = init_zdi_pins();
  hard_assert(rc == PICO_OK);

  stdio_init_all();

  while (true) {
    uint8_t r0 = read_reg_byte(0);
    printf("R0 = %02X\r\n", r0);
    sleep_ms(1000);
  }
}
