#ifndef __MSE_DRIVER__
#define __MSE_DRIVER__

#include "../base-drv/dev_transfers.h"
#include <stdint.h>

#define MOUSE_BUFFER_SIZE      4
#define MOUSE_BUFFER_SIZE_MASK 3

#define RPT_MOUSE_BUFFER_SIZE      4
#define RPT_MOUSE_BUFFER_SIZE_MASK 3

typedef struct {
  uint8_t buttons;
  int8_t  x;
  int8_t  y;
} usb_mouse_report_t;

typedef struct {
  uint8_t buttons;
  int24_t x;
  int24_t y;
} usb_mouse_report_ex_t;

typedef struct {
  usb_mouse_report_t report;
  uint8_t            reserved[5];
} usb_mouse_incoming_report_t;

typedef struct {
  uint8_t buttons;
  int24_t x;
  int24_t y;
} mouse_report_t;

extern device_config_boot_hid_t *mouse_config;

extern usb_error_t usb_mse_init(const uint8_t dev_index);

#endif
