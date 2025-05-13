#ifndef __EZ80_WOLF3D__
#define __EZ80_WOLF3D__

#include <ez80-firmware-usb.h>
#include <ez80-firmware.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

extern ez80_usb_mse_report_ex_t usb_mouse_report;
extern usb_kyb_event_t          usb_key;
extern uint24_t                 _time_count;

extern void     ez80_startup();
extern bool     io_mouse_init();

static inline uint24_t GetTimeCount() { return ez80_timers_ticks_get(); }

static inline void SetTimeCount(uint24_t new_count) { ez80_timers_ticks_set(new_count); }

static inline void SubTimeCount(uint24_t new_count) { ez80_timers_ticks_set(ez80_timers_ticks_get() - new_count); }

static inline uint8_t io_mouse_buttons() {
  ez80_usb_mse_state(&usb_mouse_report);
  return usb_mouse_report.buttons;
}

static inline void io_mouse_move(int24_t *const total_mouse_x, int24_t *const total_mouse_y) {
  ez80_usb_mse_read(&usb_mouse_report);

  *total_mouse_x += usb_mouse_report.x;
  *total_mouse_y += usb_mouse_report.y;
}

static inline void io_mouse_poll() { ez80_usb_mse_read(&usb_mouse_report); }

static inline void io_keyboard_flush() {
  ez80_usb_kyb_flush();
}

static inline bool io_keyboard_event(bool *const keyboard_states, uint8_t *last_scanned_code, char *last_ascii_code) {
  uint8_t result = ez80_usb_kyb_event(&usb_key);

  if (result == 0)
    return false;

  /* Quick Exit option */
  if (usb_key.key_code == USB_KEY_DELETE) {
    printf("DELETE KEY ABORT!\r\n");
    exit(1);
  }


  if (usb_key.key_down) {
    *last_scanned_code = usb_key.key_code;
    *last_ascii_code   = usb_key.key_ascii;

    keyboard_states[*last_scanned_code] = 1;
    return true;
  }

  keyboard_states[usb_key.key_code] = 0;
  return true;
}

#endif
