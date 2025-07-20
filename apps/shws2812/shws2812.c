
#include "colour_conversion.h"
#include <cpm.h>
#include <ez80-firmware.h>
#include <ez80.h>
#include <stdio.h>
#include <v99x8-super.h>

// effects copied from: https://www.tweaking4all.com/hardware/arduino/arduino-all-ledstrip-effects-in-one/

const RGB red   = {0xff, 0, 0};
const RGB black = {0, 0, 0};

uint8_t num_leds;

/**
 * implement a Caterpillar like effect for the strip
 *
 */
void effect_caterpillar(const uint8_t caterpillar_len) {

  RGB     rgb;
  uint8_t pos = 0;
  HSL     hsl = {0, 0.8, 0.25};

  convertHSLtoRGB(hsl, &rgb);

  uint24_t t = start_sync_ms(250);

  while (1) {
    wait_sync_ms(t);
    t = start_sync_ms(200);

    WS2812_LEDIDX = 0;
    for (int i = 0; i < num_leds; i++) {
      int endpos = pos + caterpillar_len;
      if ((i >= pos && i < endpos) || ((endpos > num_leds) && (i < endpos - num_leds || i >= pos))) {

        WS2812_LEDVAL = rgb.red;
        WS2812_LEDVAL = rgb.green;
        WS2812_LEDVAL = rgb.blue;
      } else {
        WS2812_LEDVAL = 0;
        WS2812_LEDVAL = 0;
        WS2812_LEDVAL = 0;
      }

      hsl.h += 0.005;
      if (hsl.h > 1.0)
        hsl.h = 0;
      convertHSLtoRGB(hsl, &rgb);
    }

    pos++;
    if (pos >= num_leds)
      pos = 0;

    if (cpm_c_rawio() != 0)
      break;
  }
}

void effect_theater_chase(const RGB rgb, uint8_t delay) {
  while (1) {
    for (uint8_t j = 0; j < 10; j++) { // do 10 cycles of chasing
      for (uint8_t q = 0; q < 3; q++) {
        for (uint8_t i = 0; i < num_leds; i = i + 3)
          ws2812_set_pixel(i + q, rgb); // turn every third pixel on

        if (cpm_c_rawio() != 0)
          return;

        sleep_ms(delay);

        if (cpm_c_rawio() != 0)
          return;

        for (uint8_t i = 0; i < num_leds; i = i + 3)
          ws2812_set_pixel(i + q, black); // turn every third pixel off
      }
    }
  }
}

int main() {
  char input[4];

  printf("Test of WS2812 led strip for SUPER HDMI Module\n");

  printf("Enter number of attached leds? ");

  int i;
  scanf("%d", &i);
  num_leds = i;

  printf("\nNumber is %d\n", num_leds);
  WS2812_LEDCNT = num_leds;

  // Effect selection menu
  printf("\nSelect effect to display:\n");
  printf("  a) Theater Chase\n");
  printf("  b) Caterpillar\n");
  printf("Enter choice: ");

  fgets(input, 4, stdin);
  printf("\r\n");

  char sel = input[0];
  if (sel >= 'A' && sel <= 'Z')
    sel += 'a' - 'A'; // to lower

  printf("Press any key to abort\n");

  if (sel == 'a')
    effect_theater_chase(red, 100);

  else if (sel == 'b')
    effect_caterpillar(5);

  else
    printf("Invalid choice. Exiting.\n");
}
