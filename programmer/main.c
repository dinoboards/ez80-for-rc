#include "pico/stdlib.h"
#include <stdio.h>
#ifdef CYW43_WL_GPIO_LED_PIN
#include "main.h"
#include "pico/cyw43_arch.h"
#endif
#include "read_line.h"

#define LOW  false
#define HIGH true

#define ZDI_ZCL_PIN 15
#define ZDI_ZDA_PIN 22
#define GPIO_22     22

#define ZDI_READ     1
#define ZDI_WRITE    0
#define ZDI_CONTINUE 0
#define ZDI_ENDED    1

#define IN_RAM __attribute__((noinline, long_call, section(".time_critical")))

#define DELAY_SMALL() __asm volatile("nop\n")

void IN_RAM pin_low(uint8_t pin);
void IN_RAM pin_high(uint8_t pin);

int init_zdi_pins(void) {
  gpio_set_function(ZDI_ZDA_PIN, GPIO_FUNC_NULL);
  gpio_set_function(ZDI_ZCL_PIN, GPIO_FUNC_NULL);

  gpio_set_slew_rate(ZDI_ZDA_PIN, GPIO_SLEW_RATE_SLOW);
  gpio_set_slew_rate(ZDI_ZCL_PIN, GPIO_SLEW_RATE_SLOW);

  gpio_set_drive_strength(ZDI_ZDA_PIN, GPIO_DRIVE_STRENGTH_12MA);
  gpio_set_drive_strength(ZDI_ZCL_PIN, GPIO_DRIVE_STRENGTH_12MA);

  gpio_init(ZDI_ZDA_PIN);
  gpio_init(ZDI_ZCL_PIN);

  gpio_set_dir(ZDI_ZDA_PIN, GPIO_IN);
  gpio_set_dir(ZDI_ZCL_PIN, GPIO_OUT);
  pin_high(ZDI_ZCL_PIN);

  gpio_set_pulls(ZDI_ZDA_PIN, true, false);

  return PICO_OK;
}

void IN_RAM pin_high(uint8_t pin) { gpio_put(pin, HIGH); }

void IN_RAM pin_low(uint8_t pin) { gpio_put(pin, LOW); }

void IN_RAM start_condition() {
  gpio_set_dir(ZDI_ZDA_PIN, GPIO_OUT);

  pin_high(ZDI_ZDA_PIN);

  pin_high(ZDI_ZCL_PIN);

  pin_low(ZDI_ZDA_PIN);
}

void IN_RAM send_bit(const bool bit) {
  pin_low(ZDI_ZCL_PIN);

  if (bit)
    pin_high(ZDI_ZDA_PIN);
  else
    pin_low(ZDI_ZDA_PIN);

  pin_high(ZDI_ZCL_PIN);
}

uint8_t IN_RAM read_bit() {
  uint8_t result = 0;

  pin_low(ZDI_ZCL_PIN);

  result = gpio_get(ZDI_ZDA_PIN) ? 1 : 0;

  pin_high(ZDI_ZCL_PIN);

  return result;
}

void IN_RAM send_single_read_address(const uint8_t dat) {
  send_bit(dat & 0x40);
  send_bit(dat & 0x20);
  send_bit(dat & 0x10);
  send_bit(dat & 0x08);
  send_bit(dat & 0x04);
  send_bit(dat & 0x02);
  send_bit(dat & 0x01);
  send_bit(ZDI_READ);
  send_bit(1);
  gpio_set_dir(ZDI_ZDA_PIN, GPIO_IN);
}

uint8_t IN_RAM read_single_byte() {
  uint8_t result = 0;

  result |= read_bit();
  result <<= 1;
  result |= read_bit();
  result <<= 1;
  result |= read_bit();
  result <<= 1;
  result |= read_bit();
  result <<= 1;
  result |= read_bit();
  result <<= 1;
  result |= read_bit();
  result <<= 1;
  result |= read_bit();
  result <<= 1;
  result |= read_bit();

  send_bit(1);

  return result;
}

uint8_t IN_RAM read_reg_byte(const uint8_t reg_addr) {
  start_condition();
  send_single_read_address(reg_addr);
  return read_single_byte();
}

int main() {
  // int rc = init_zdi_pins();
  // hard_assert(rc == PICO_OK);

  stdio_init_all();

  while (true) {
    read_line();
    printf("\r\n%s\n", input_buffer);
  }

  // while (true) {
  //   gpio_set_dir(ZDI_ZDA_PIN, GPIO_IN);

  //   while (gpio_get(ZDI_ZDA_PIN) == 0 || gpio_get(ZDI_ZCL_PIN) == 0)
  //     ;

  //   uint8_t r0 = read_reg_byte(3);
  //   gpio_set_dir(ZDI_ZDA_PIN, GPIO_IN);

  //   printf("R0 = %02X\r\n", r0);
  //   sleep_ms(500);
  // }
}
