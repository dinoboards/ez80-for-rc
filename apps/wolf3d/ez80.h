#ifndef __EZ80_WOLF3D__
#define __EZ80_WOLF3D__

#include <stdbool.h>
#include <stdint.h>
#include <ez80-firmware-usb.h>

extern ez80_usb_mse_report_ex_t usb_mouse_report;

extern bool io_mouse_init();

static inline uint8_t io_mouse_buttons() {
  ez80_usb_mse_state(&usb_mouse_report);
  return usb_mouse_report.buttons;
}

#endif
