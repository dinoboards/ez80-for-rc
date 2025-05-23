#include "class_hid.h"
#include <stdint.h>

const setup_packet_t cmd_hid_set = {0x21, HID_SET_PROTOCOL, {0, 0}, {0, 0}, 0};

usb_error_t hid_set_protocol(const device_config_boot_hid_t *const dev, const uint8_t protocol) {
  setup_packet_t cmd;
  cmd = cmd_hid_set;

  cmd.bRequest  = HID_SET_PROTOCOL;
  cmd.bValue[0] = protocol;

  return usb_control_transfer(&cmd, NULL, dev->address, dev->max_packet_size);
}

usb_error_t hid_set_idle(const device_config_boot_hid_t *const dev, const uint8_t duration) {
  setup_packet_t cmd;
  cmd = cmd_hid_set;

  cmd.bRequest  = HID_SET_IDLE;
  cmd.bValue[0] = duration;

  return usb_control_transfer(&cmd, NULL, dev->address, dev->max_packet_size);
}
