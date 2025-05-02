#ifndef __EZ80_WOLF3D__
#define __EZ80_WOLF3D__

#include <ez80-firmware-usb.h>
#include <stdbool.h>
#include <stdint.h>

extern ez80_usb_mse_report_ex_t usb_mouse_report;
extern ez80_usb_kyb_event_t     usb_key;

extern bool io_mouse_init();

static inline uint8_t io_mouse_buttons() {
  ez80_usb_mse_state(&usb_mouse_report);
  return usb_mouse_report.buttons;
}

static inline bool io_keyboard_event(bool *const keyboard_states, uint8_t *last_scanned_code) {
  uint8_t result = ez80_usb_kyb_event(&usb_key);

  if (result == 0)
    return false;

  if (usb_key.key_down) {
    *last_scanned_code                  = usb_key.key_code;
    keyboard_states[*last_scanned_code] = 1;
    return true;
  }

  keyboard_states[usb_key.key_code] = 0;
  return true;
}

#endif
