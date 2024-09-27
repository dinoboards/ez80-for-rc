#ifndef __ZDI_H
#define __ZDI_H

#include <stdbool.h>
#include <stdint.h>

#define LOW  false
#define HIGH true

#define ZDI_ZCL_PIN   15
#define ZDI_ZDA_PIN   14
#define ZDI_RESET_PIN 16
#define ZDI_POWER_PIN 17

// #define GPIO_22     22

#define ZDI_READ     1
#define ZDI_WRITE    0
#define ZDI_CONTINUE 0
#define ZDI_ENDED    1

#define ZDI_ID_L   0x00
#define ZDI_ID_H   0x01
#define ZDI_ID_REV 0x02

#define IN_RAM __attribute__((noinline, long_call, section(".time_critical")))

#define DELAY_SMALL() __asm volatile("nop\n")

typedef void (*report_connection_fn_t)(
    bool power_detected, bool reset_detected, bool zda_detected, bool zcl_detected, bool connection_detected);
typedef void (*report_non_connection_fn_t)(void);
typedef void (*report_ez80_id_fn_t)(uint8_t zdi_id_low, uint8_t zdi_id_high, uint8_t zdi_id_rev);
typedef void (*report_ez80_id_failed_fn_t)(void);

void init_pins(void);
void wait_for_detected_zdi_connection(const report_connection_fn_t, const report_non_connection_fn_t);
void configure_for_zdi_interface(void);
bool zdi_connection_lost(void);
void wait_for_valid_identity(const report_ez80_id_fn_t, const report_ez80_id_failed_fn_t);

void IN_RAM    pin_low(uint8_t pin);
void IN_RAM    pin_high(uint8_t pin);
uint8_t IN_RAM read_reg_byte(const uint8_t reg_addr);

#endif
