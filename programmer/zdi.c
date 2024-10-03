#include "zdi.h"
#include "ez80f92.h"
#include "pico/stdlib.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>

static uint8_t  zdi_brk_ctl   = 0;
static uint32_t cpu_frequency = 20000000;

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
  gpio_set_dir(ZDI_ZDA_PIN, GPIO_IN);

  while (gpio_get(ZDI_ZDA_PIN) == 0 || gpio_get(ZDI_ZCL_PIN) == 0)
    ;
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

uint8_t IN_RAM read_single_byte(const uint8_t separator_id) {
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

  send_bit(separator_id);

  return result;
}

void IN_RAM write_single_byte(const uint8_t value, const uint8_t separator_id) {
  send_bit(value & 0x80);
  send_bit(value & 0x40);
  send_bit(value & 0x20);
  send_bit(value & 0x10);
  send_bit(value & 0x08);
  send_bit(value & 0x04);
  send_bit(value & 0x02);
  send_bit(value & 0x01);
  send_bit(separator_id);
}

uint8_t IN_RAM zdi_rd_reg(const uint8_t reg_addr, const uint8_t separator_id) {
  wait_for_zda();
  start_condition();
  send_single_read_address(reg_addr);
  return read_single_byte(separator_id);
}

void IN_RAM zdi_wr_reg(const uint8_t reg_addr, const uint8_t value, const uint8_t separator_id) {
  wait_for_zda();
  start_condition();
  send_single_write_address(reg_addr);
  write_single_byte(value, separator_id);
}

void zdi_debug_break() {
  zdi_brk_ctl |= 0b10000000;
  zdi_wr_reg_byte(ZDI_WR_BRK_CTL, zdi_brk_ctl);
}

void zdi_debug_continue() {
  zdi_brk_ctl &= 0b01111111;
  zdi_wr_reg_byte(ZDI_WR_BRK_CTL, zdi_brk_ctl);
}

#define INSTR_DI      0x5F
#define INSTR_LD_A_NN 0x3E
#define OUT0_NN_A     ((int[]){0xED, 0x39})

void zdi_instr_di() { zdi_wr_reg_byte(ZDI_WR_IS0, INSTR_DI); }

void zdi_load_a_nn(uint8_t nn) {
  zdi_wr_reg_byte(ZDI_WR_IS1, nn);
  zdi_wr_reg_byte(ZDI_WR_IS0, INSTR_LD_A_NN);
}

void zdi_out0_nn_a(uint8_t nn) {
  zdi_wr_reg_byte(ZDI_WR_IS2, nn);
  zdi_wr_reg_byte(ZDI_WR_IS1, OUT0_NN_A[1]);
  zdi_wr_reg_byte(ZDI_WR_IS0, OUT0_NN_A[0]);
}

void zdi_set_mode_adl() { zdi_wr_reg_byte(ZDI_WR_RW_CTL, RW_CTL_SET_ADL); }

void zdi_set_mode_z80() { zdi_wr_reg_byte(ZDI_WR_RW_CTL, RW_CTL_RESET_ADL); }

void zdi_full_reset() {
  zdi_debug_break();
  zdi_instr_di();

  zdi_set_mode_adl();

  zdi_load_a_nn(0x00);
  zdi_out0_nn_a(FLASH_ADDR_U); // flash at 0x00xxxx
  zdi_load_a_nn(0x88);         // flash enabled, 4 wait state
  zdi_out0_nn_a(FLASH_CTRL);

  uint8_t fdiv = calculate_flash_div(cpu_frequency);
  zdi_load_a_nn(FLASH_KEY_UNLOCK_1);
  zdi_out0_nn_a(FLASH_KEY);
  zdi_load_a_nn(FLASH_KEY_UNLOCK_2);
  zdi_out0_nn_a(FLASH_KEY);
  zdi_load_a_nn(fdiv);
  zdi_out0_nn_a(FLASH_FDIV);

  zdi_wr_reg_byte(ZDI_WR_DATA_L, 0x00);
  zdi_wr_reg_byte(ZDI_WR_DATA_H, 0x00);
  zdi_wr_reg_byte(ZDI_WR_DATA_U, 0x00);
  zdi_wr_reg_byte(ZDI_WR_RW_CTL, RW_CTL_REG_PC | RW_CTL_WR);
}

void zdi_set_cpu_freq(const uint32_t freq) { cpu_frequency = freq; }

void zdi_flash_write_enable(void) {
  zdi_load_a_nn(FLASH_KEY_UNLOCK_1);
  zdi_out0_nn_a(FLASH_KEY);
  zdi_load_a_nn(FLASH_KEY_UNLOCK_2);
  zdi_out0_nn_a(FLASH_KEY);
  zdi_load_a_nn(0);
  zdi_out0_nn_a(FLASH_PROT);
}

void zdi_flash_write_disable(void) {
  zdi_load_a_nn(FLASH_KEY_UNLOCK_1);
  zdi_out0_nn_a(FLASH_KEY);
  zdi_load_a_nn(FLASH_KEY_UNLOCK_2);
  zdi_out0_nn_a(FLASH_KEY);
  zdi_load_a_nn(0xFF);
  zdi_out0_nn_a(FLASH_PROT);
}

void zdi_erase_flash(void) {
  // issue full erase command
  zdi_load_a_nn(FLASH_PGCTL_MASS_ERASE);
  zdi_out0_nn_a(FLASH_PGCTL);

  // we could poll that status or just sleep
  sleep_ms(500);
}

void IN_RAM zdi_write_byte(const uint32_t address, const uint8_t data) {
  // set PC to address
  zdi_wr_reg_byte(ZDI_WR_DATA_L, address & 0xFF);
  zdi_wr_reg_byte(ZDI_WR_DATA_H, (address >> 8) & 0xFF);
  zdi_wr_reg_byte(ZDI_WR_DATA_U, (address >> 16) & 0xFF);
  zdi_wr_reg_byte(ZDI_WR_RW_CTL, RW_CTL_REG_PC | RW_CTL_WR);

  sleep_us(3);
  zdi_wr_reg_byte(ZDI_WR_MEM, data);
}

uint8_t IN_RAM zdi_read_byte(const uint32_t address) {
  // set PC to address
  zdi_wr_reg_byte(ZDI_WR_DATA_L, address & 0xFF);
  zdi_wr_reg_byte(ZDI_WR_DATA_H, (address >> 8) & 0xFF);
  zdi_wr_reg_byte(ZDI_WR_DATA_U, (address >> 16) & 0xFF);
  zdi_wr_reg_byte(ZDI_WR_RW_CTL, RW_CTL_REG_PC | RW_CTL_WR);

  sleep_us(3);
  return zdi_rd_reg_byte(ZDI_RD_MEM);
}

uint32_t read_reg_pc() {
  zdi_wr_reg_byte(ZDI_WR_RW_CTL, RW_CTL_REG_PC);

  uint8_t l = zdi_rd_reg_byte(ZDI_RD_DATA_L);
  uint8_t h = zdi_rd_reg_byte(ZDI_RD_DATA_H);
  uint8_t u = zdi_rd_reg_byte(ZDI_RD_DATA_U);

  return (u << 16) | (h << 8) | l;
}

void write_reg_pc(const uint32_t pc) {
  zdi_wr_reg_byte(ZDI_WR_DATA_L, pc & 0xFF);
  zdi_wr_reg_byte(ZDI_WR_DATA_H, (pc >> 8) & 0xFF);
  zdi_wr_reg_byte(ZDI_WR_DATA_U, (pc >> 16) & 0xFF);
  zdi_wr_reg_byte(ZDI_WR_RW_CTL, RW_CTL_REG_PC | RW_CTL_WR);
}

void zdi_flash_write_bytes(const uint32_t address, const uint8_t *data, const uint16_t len) {

  zdi_set_mode_adl();

  for (int i = 0; i < len; i++) {
    zdi_write_byte(address + i, data[i]);
    sleep_ms(1);
  }
}
