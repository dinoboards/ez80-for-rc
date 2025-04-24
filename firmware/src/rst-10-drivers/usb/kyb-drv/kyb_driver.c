#include "kyb_driver.h"
#include "../base-drv/critical-section.h"
#include "../base-drv/dev_transfers.h"
#include "../base-drv/usb_state.h"
#include "class_hid.h"
#include "class_hid_keyboard.h"
#include <stdbool.h>
#include <stdint.h>

static bool                    caps_lock_engaged = true;
static device_config_keyboard *keyboard_config   = 0;

static uint8_t buffer[KEYBOARD_BUFFER_SIZE] = {0};
static uint8_t write_index                  = 0;
static uint8_t read_index                   = 0;

// static uint8_t           alt_write_index               = 0;
// static uint8_t           alt_read_index                = 0;
// static keyboard_report_t reports[KEYBOARD_BUFFER_SIZE] = {{0}};

// static keyboard_report_t *queued_report = NULL;
keyboard_report_t report   = {0};
keyboard_report_t previous = {0};

#define DI asm("DI")
#define EI asm("EI")

// static void report_put() {
//   uint8_t next_write_index = (alt_write_index + 1) & KEYBOARD_BUFFER_SIZE_MASK;

//   if (next_write_index != alt_read_index) { // Check if buffer is not full
//     reports[alt_write_index] = report;
//     alt_write_index          = next_write_index;
//   }
// }

static void keyboard_buf_put(const uint8_t indx) {
  unsigned char c;
  uint8_t       next_write_index;
  uint8_t       i        = 6;
  uint8_t      *a        = previous.keyCode;
  const uint8_t key_code = report.keyCode[indx];

  if (key_code >= 0x80 || key_code == 0)
    return; // ignore ???

  // if already reported, just skip it
  if (previous.keyCode[indx] == key_code)
    return;

  if (key_code == KEY_CODE_CAPS_LOCK) {
    caps_lock_engaged = !caps_lock_engaged;
    return;
  }

  c = scancode_to_char(report.bModifierKeys, key_code, caps_lock_engaged);

  if (c == 0)
    return;

  next_write_index = (write_index + 1) & KEYBOARD_BUFFER_SIZE_MASK;
  if (next_write_index != read_index) { // Check if buffer is not full
    buffer[write_index] = c;
    write_index         = next_write_index;
  }
}

// uint16_t usb_kyb_buf_size() {
//   uint8_t size;
//   uint8_t alt_size;

//   DI;

//   if (alt_write_index >= alt_read_index)
//     alt_size = alt_write_index - alt_read_index;
//   else
//     alt_size = KEYBOARD_BUFFER_SIZE - alt_read_index + alt_write_index;

//   if (alt_size != 0)
//     alt_read_index = (alt_read_index + 1) & KEYBOARD_BUFFER_SIZE_MASK;

//   if (write_index >= read_index)
//     size = write_index - read_index;
//   else
//     size = KEYBOARD_BUFFER_SIZE - read_index + write_index;

//   EI;
//   return (uint16_t)alt_size << 8 | (uint16_t)size;
// }

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
  write_index = read_index /*= alt_write_index = alt_read_index*/ = 0;

  i = sizeof(previous);
  a = (uint8_t *)&previous;
  b = (uint8_t *)&report;

  do {
    *a++ = 0;
    *b++ = 0;
  } while (--i != 0);

  EI;
  return 0;
}

void usb_kyb_tick(void) {
  uint8_t   i = 6;
  usb_error result;

  if (is_in_critical_section())
    return;

  ch_configure_nak_retry_disable();
  result = usbdev_dat_in_trnsfer_0((device_config *)keyboard_config, (uint8_t *)&report, 8);
  ch_configure_nak_retry_3s();
  if (result == 0) {
    if (report_diff()) {
      // report_put();
      do {
        keyboard_buf_put(i - 1);
      } while (--i != 0);
      previous = report;
    }
  }
}

usb_error usb_kyb_init(const uint8_t dev_index) {
  usb_error result;
  keyboard_config = (device_config_keyboard *)get_usb_device_config(dev_index);

  if (keyboard_config == NULL)
    return USB_ERR_OTHER;

  CHECK(hid_set_protocol(keyboard_config, 1));
  CHECK(hid_set_idle(keyboard_config, 0x80));

  usb_kyb_install_timer_tick();

done:
  return result;
}
