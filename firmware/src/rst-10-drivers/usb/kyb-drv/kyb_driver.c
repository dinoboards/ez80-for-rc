#include "kyb_driver.h"
#include "../base-drv/class_hid.h"
#include "../base-drv/critical-section.h"
#include "../base-drv/dev_transfers.h"
#include "../base-drv/usb_state.h"
#include "class_hid_keyboard.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

static bool               caps_lock_engaged = true;
device_config_boot_hid_t *keyboard_config   = NULL;

static uint8_t buffer[KEYBOARD_BUFFER_SIZE] = {0};
static uint8_t write_index                  = 0;
static uint8_t read_index                   = 0;

uint8_t           kyb_rpt_write_index                   = 0;
uint8_t           kyb_rpt_read_index                    = 0;
keyboard_report_t kyb_reports[RPT_KEYBOARD_BUFFER_SIZE] = {{0}};

keyboard_report_t kyb_report   = {0};
keyboard_report_t kyb_previous = {0};

#define DI asm("DI")
#define EI asm("EI")

void keyboard_report_put() {
  uint8_t next_write_index = (kyb_rpt_write_index + 1) & RPT_KEYBOARD_BUFFER_SIZE_MASK;

  if (next_write_index != kyb_rpt_read_index) { // Check if buffer is not full
    kyb_reports[kyb_rpt_write_index] = kyb_report;
    kyb_rpt_write_index              = next_write_index;
  }
}

void keyboard_buf_put(const uint8_t indx) {
  unsigned char c;
  uint8_t       next_write_index;
  uint8_t       i        = 6;
  uint8_t      *a        = kyb_previous.keyCode;
  const uint8_t key_code = kyb_report.keyCode[indx];

  if (key_code >= 0x80 || key_code == 0)
    return; // ignore ???

  // if already reported, just skip it
  if (kyb_previous.keyCode[indx] == key_code)
    return;

  if (key_code == KEY_CODE_CAPS_LOCK) {
    caps_lock_engaged = !caps_lock_engaged;
    return;
  }

  c = scancode_to_char(kyb_report.bModifierKeys, key_code, caps_lock_engaged);

  if (c == 0)
    return;

  next_write_index = (write_index + 1) & KEYBOARD_BUFFER_SIZE_MASK;
  if (next_write_index != read_index) { // Check if buffer is not full
    buffer[write_index] = c;
    write_index         = next_write_index;
  }
}

uint8_t usb_kyb_status() {
  uint8_t size;

  DI;

  if (write_index >= read_index)
    size = write_index - read_index;
  else
    size = KEYBOARD_BUFFER_SIZE - read_index + write_index;

  EI;
  return size;
}

uint24_t usb_kyb_read() {
  uint8_t c;

  if (write_index == read_index) // Check if buffer is empty
    return 0x00FF00;             // H = -1, L = 0

  DI;
  c          = buffer[read_index];
  read_index = (read_index + 1) & KEYBOARD_BUFFER_SIZE_MASK;
  EI;

  /* H = 0, L = ascii char */
  return c;
}

uint8_t usb_kyb_flush() {
  uint8_t  i;
  uint8_t *a;
  uint8_t *b;

  DI;
  write_index = read_index = kyb_rpt_write_index = kyb_rpt_read_index = 0;

  i = sizeof(kyb_previous);
  a = (uint8_t *)&kyb_previous;
  b = (uint8_t *)&kyb_report;

  do {
    *a++ = 0;
    *b++ = 0;
  } while (--i != 0);

  EI;
  return 0;
}

usb_error_t usb_kyb_init(const uint8_t dev_index) {
  usb_error_t               result;
  device_config_boot_hid_t *config;

  caps_lock_engaged = true;
  keyboard_config   = NULL;
  memset(buffer, 0, sizeof(buffer));
  write_index = read_index = kyb_rpt_write_index = kyb_rpt_read_index = 0;
  memset(&kyb_reports, 0, sizeof(kyb_reports));
  memset(&kyb_report, 0, sizeof(kyb_report));
  memset(&kyb_previous, 0, sizeof(kyb_previous));

  config = (device_config_boot_hid_t *)get_usb_device_config(dev_index);

  if (config == NULL)
    return USB_ERROR_DEVICE_NOT_FOUND;

  if (config->type != USB_IS_KEYBOARD)
    return USB_ERROR_DEVICE_NOT_FOUND;

  CHECK(hid_set_protocol(config, 1));
  CHECK(hid_set_idle(config, 0x80));

  keyboard_config = config;

done:
  return result;
}

uint8_t usb_kyb_rpt_que_size() {
  uint8_t alt_size = 0;
  DI;

  if (kyb_rpt_write_index >= kyb_rpt_read_index)
    alt_size = kyb_rpt_write_index - kyb_rpt_read_index;
  else
    alt_size = RPT_KEYBOARD_BUFFER_SIZE - kyb_rpt_read_index + kyb_rpt_write_index;

  if (alt_size != 0)
    kyb_rpt_read_index = (kyb_rpt_read_index + 1) & RPT_KEYBOARD_BUFFER_SIZE_MASK;

  EI;
  return alt_size;
}
