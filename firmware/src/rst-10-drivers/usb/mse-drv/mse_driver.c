#include "mse_driver.h"
#include "../base-drv/ch376.h"
#include "../base-drv/class_hid.h"
#include "../base-drv/dev_transfers.h"
#include "../base-drv/usb_state.h"
#include <stdbool.h>
#include <stdint.h>

#define DI asm("DI")
#define EI asm("EI")

device_config_boot_hid_t *mouse_config = NULL;

static usb_mouse_report_ex_t buffer[MOUSE_BUFFER_SIZE] = {0};
static uint8_t               write_index               = 0;
static uint8_t               read_index                = 0;

// uint8_t           mse_rpt_write_index                   = 0;
// uint8_t           mse_rpt_read_index                    = 0;
// mouse_report_t mse_reports[RPT_MOUSE_BUFFER_SIZE] = {{0}};

usb_mouse_incoming_report_t mse_report   = {0};
usb_mouse_report_t          mse_previous = {0};

usb_error_t usb_mse_init(const uint8_t dev_index) {
  uint8_t                   result;
  device_config_boot_hid_t *config;

  mouse_config = NULL;

  config = (device_config_boot_hid_t *)(dev_index == 0 ? find_by_device_type(USB_IS_MOUSE) : get_usb_device_config(dev_index));

  if (config == NULL)
    return USB_ERROR_DEVICE_NOT_FOUND;

  if (config->type != USB_IS_MOUSE)
    return USB_ERROR_DEVICE_NOT_FOUND;

  CHECK(hid_set_protocol(config, 1));
  CHECK(hid_set_idle(config, 0x80));

  mouse_config = config;

  asm(" XREF _system_timer_isr");
  asm(" XREF _usb_tick_sr");
  asm("LD	HL, _system_timer_isr");
  asm("LD	DE, _usb_tick_sr");
  asm("DI");
  asm("LD   (HL), %CD");
  asm("INC  HL");
  asm("LD	  (HL), DE");
  asm("EI");

done:
  return USB_ERR_OK;
}

void mouse_report_put() {
  uint8_t next_write_index = (write_index + 1) & MOUSE_BUFFER_SIZE_MASK;

  /* if not empty, and buttons are same, just updated the previously */
  /* written buffered report                                         */
  if (read_index != write_index) {
    uint8_t previous_index = (write_index - 1) & MOUSE_BUFFER_SIZE_MASK;
    if (buffer[previous_index].buttons == mse_report.report.buttons) {
      buffer[previous_index].x += mse_report.report.x;
      buffer[previous_index].y += mse_report.report.y;
      return;
    }
  }

  if (next_write_index != read_index) { // Check if buffer is not full
    buffer[write_index].buttons = mse_report.report.buttons;
    buffer[write_index].x       = mse_report.report.x;
    buffer[write_index].y       = mse_report.report.y;
    write_index                 = next_write_index;
  }
}

static bool mouse_report_diff() {
  if (mse_report.report.buttons != mse_previous.buttons)
    return true;

  if (mse_report.report.x != mse_previous.x)
    return true;

  if (mse_report.report.y != mse_previous.y)
    return true;

  return false;
}

// needs to return bool, buttons, x, and y
uint8_t usb_mse_read(usb_mouse_report_ex_t *rpt) {
  if (write_index == read_index) // Check if buffer is empty
    return 255;                  // empty

  DI;
  *rpt       = buffer[read_index];
  read_index = (read_index + 1) & MOUSE_BUFFER_SIZE_MASK;
  EI;

  return 0; // item returned
}

void mouse_inject(void) {
  if (mouse_report_diff() == 0)
    return;

  mouse_report_put();

  mse_previous = mse_report.report;
}
