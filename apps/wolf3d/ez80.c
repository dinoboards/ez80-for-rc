#include "ez80.h"
#include <ez80-firmware-usb.h>
#include <ez80-firmware.h>
#include <stdio.h>
#include <stdlib.h>

ez80_usb_mse_report_ex_t usb_mouse_report = {0};
ez80_usb_kyb_event_t     usb_key          = {0};
uint24_t                 _time_count      = 0;
static uint24_t          previous_t       = 0;

bool io_mouse_init() {
  const uint8_t mouse_index = ez80_usb_find_device_index(USB_IS_MOUSE);

  if (mouse_index == 0)
    return false;

  if (ez80_usb_mse_init(0) != USB_ERR_OK)
    return false;

  printf("Found Mouse: ");

  char str_buffer[64];

  ez80_usb_get_manufacturer(mouse_index, str_buffer, sizeof(str_buffer));
  printf("'%s', ", str_buffer);

  ez80_usb_get_product(mouse_index, str_buffer, sizeof(str_buffer));
  printf("'%s'\r\n", str_buffer);

  return true;
}

uint24_t tm_tick_get() {
  uint24_t t = ez80_timers_ticks_get();

  uint24_t diff = t - previous_t;

  previous_t = t;

  _time_count += diff;

  return _time_count;
}

static uint8_t original_timer_tick_freq = 50;

static void ez80_restore() { ez80_timers_freq_tick_set(original_timer_tick_freq); }

void ez80_startup() {
  original_timer_tick_freq = ez80_timers_freq_tick_get();
  atexit(ez80_restore);

  ez80_timers_freq_tick_set(70);
}
