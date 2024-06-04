#include <eZ80.h>

#define DIGITAL_IO_LEDS		(*(XSFR)0xFF00)

void toggle_off_leds() {
  DIGITAL_IO_LEDS = 0;
}

void toggle_on_leds() {
  DIGITAL_IO_LEDS = 255;
}

void set_leds() {
  DIGITAL_IO_LEDS = DIGITAL_IO_LEDS;
}
