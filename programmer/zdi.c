#include "zdi.h"
#include "pico/stdlib.h"
#include <stdio.h>
#include <string.h>

static uint8_t zdi_brk_ctl = 0;

void IN_RAM pin_low(uint8_t pin);
void IN_RAM pin_high(uint8_t pin);

void zdi_init_pins(void) {
  gpio_init(ZDI_ZDA_PIN);
  gpio_init(ZDI_ZCL_PIN);
  gpio_init(ZDI_RESET_PIN);
  gpio_init(ZDI_POWER_PIN);

  // gpio_set_function(ZDI_ZDA_PIN, GPIO_FUNC_NULL);
  // gpio_set_function(ZDI_ZCL_PIN, GPIO_FUNC_NULL);
  // gpio_set_function(ZDI_RESET_PIN, GPIO_FUNC_NULL);
  // gpio_set_function(ZDI_POWER_PIN, GPIO_FUNC_NULL);

  // gpio_set_slew_rate(ZDI_ZDA_PIN, GPIO_SLEW_RATE_SLOW);
  // gpio_set_slew_rate(ZDI_ZCL_PIN, GPIO_SLEW_RATE_SLOW);

  // gpio_set_drive_strength(ZDI_ZDA_PIN, GPIO_DRIVE_STRENGTH_12MA);
  // gpio_set_drive_strength(ZDI_ZCL_PIN, GPIO_DRIVE_STRENGTH_12MA);

  gpio_set_dir(ZDI_ZDA_PIN, GPIO_IN);
  gpio_set_dir(ZDI_ZCL_PIN, GPIO_IN);
  gpio_set_dir(ZDI_RESET_PIN, GPIO_IN);
  gpio_set_dir(ZDI_POWER_PIN, GPIO_IN);

  gpio_set_pulls(ZDI_ZDA_PIN, true, false);
}

void zdi_wait_for_connection(const report_connection_fn_t     report_connection_fn,
                             const report_non_connection_fn_t report_non_connection_fn) {

  bool connected = false;

  do {
    bool power_detected = gpio_get(ZDI_POWER_PIN);
    bool reset_detected = gpio_get(ZDI_RESET_PIN);
    bool zda_detected   = gpio_get(ZDI_ZDA_PIN);
    bool zcl_detected   = gpio_get(ZDI_ZCL_PIN);

    connected = power_detected && reset_detected && zda_detected && zcl_detected;
    report_connection_fn(power_detected, reset_detected, zda_detected, zcl_detected, connected);

    if (!connected) {
      sleep_ms(500);
      report_non_connection_fn();
      sleep_ms(500);
    }
  } while (!connected);
}

bool zdi_connection_lost() {
  bool power_detected = gpio_get(ZDI_POWER_PIN);
  bool reset_detected = gpio_get(ZDI_RESET_PIN);
  bool zda_detected   = gpio_get(ZDI_ZDA_PIN);
  bool zcl_detected   = gpio_get(ZDI_ZCL_PIN);

  bool connected = power_detected && reset_detected && zda_detected && zcl_detected;

  return !connected;
}

void zdi_wait_for_valid_identity(const report_ez80_id_fn_t        report_ez80_id_fn,
                                 const report_ez80_id_failed_fn_t report_ez80_id_failed_fn) {
  bool valid = false;

  do {
    uint8_t zdi_id_low  = zdi_rd_reg_byte(ZDI_RD_ID_L);
    uint8_t zdi_id_high = zdi_rd_reg_byte(ZDI_RD_ID_H);
    uint8_t zdi_id_rev  = zdi_rd_reg_byte(ZDI_RD_ID_REV);

    report_ez80_id_fn(zdi_id_low, zdi_id_high, zdi_id_rev);

    valid = zdi_id_high == 0 && zdi_id_low != 0xFF;

    if (!valid) {
      sleep_ms(500);
      report_ez80_id_failed_fn();
      sleep_ms(250);
    }
  } while (!valid);
}

void zdi_configure_pins(void) {
  gpio_set_dir(ZDI_ZCL_PIN, GPIO_OUT);
  pin_high(ZDI_ZCL_PIN);

  gpio_set_pulls(ZDI_ZDA_PIN, true, false);
}

void IN_RAM pin_high(uint8_t pin) { gpio_put(pin, HIGH); }

void IN_RAM pin_low(uint8_t pin) { gpio_put(pin, LOW); }

void wait_for_zda() {

  // gpio_set_dir(ZDI_ZDA_PIN, GPIO_IN);

  // while (gpio_get(ZDI_ZDA_PIN) == 0 || gpio_get(ZDI_ZCL_PIN) == 0)
  //   ;
}

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

void IN_RAM send_single_read_address(const uint8_t addr) {
  send_bit(addr & 0x40);
  send_bit(addr & 0x20);
  send_bit(addr & 0x10);
  send_bit(addr & 0x08);
  send_bit(addr & 0x04);
  send_bit(addr & 0x02);
  send_bit(addr & 0x01);
  send_bit(ZDI_READ);
  send_bit(1);
  gpio_set_dir(ZDI_ZDA_PIN, GPIO_IN);
}

void IN_RAM send_single_write_address(const uint8_t addr) {
  send_bit(addr & 0x40);
  send_bit(addr & 0x20);
  send_bit(addr & 0x10);
  send_bit(addr & 0x08);
  send_bit(addr & 0x04);
  send_bit(addr & 0x02);
  send_bit(addr & 0x01);
  send_bit(ZDI_WRITE);
  send_bit(1);
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

void IN_RAM write_single_byte(const uint8_t value) {
  send_bit(value & 0x80);
  send_bit(value & 0x40);
  send_bit(value & 0x20);
  send_bit(value & 0x10);
  send_bit(value & 0x08);
  send_bit(value & 0x04);
  send_bit(value & 0x02);
  send_bit(value & 0x01);
  send_bit(1);
}

uint8_t IN_RAM zdi_rd_reg_byte(const uint8_t reg_addr) {
  wait_for_zda();
  start_condition();
  send_single_read_address(reg_addr);
  return read_single_byte();
}

void IN_RAM zdi_wr_reg_byte(const uint8_t reg_addr, const uint8_t value) {
  wait_for_zda();
  start_condition();
  send_single_write_address(reg_addr);
  write_single_byte(value);
}

void zdi_debug_break() {
  zdi_brk_ctl |= 0b10000000;
  zdi_wr_reg_byte(ZDI_WR_BRK_CTL, zdi_brk_ctl);
}

void zdi_debug_continue() {
  zdi_brk_ctl &= 0b01111111;
  zdi_wr_reg_byte(ZDI_WR_BRK_CTL, zdi_brk_ctl);
}
