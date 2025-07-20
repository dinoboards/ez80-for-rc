
#include "colour_conversion.h"
#include <cpm.h>
#include <ez80-firmware.h>
#include <ez80.h>
#include <stdio.h>
#include <v99x8-super.h>

int main() {
  int num_leds;
  RGB rgb;

  printf("Test of WS2812 led strip for SUPER HDMI Module\n");

  printf("Enter number of attached leds? ");

  scanf("%d", &num_leds);

  printf("\nNumber is %d\n", num_leds);

  WS2812_LEDCNT = num_leds;

  printf("Press any key to abort\n");

  /* implement a Caterpillar like effect for the strip */

  int caterpillar_len = 6; // Length of the caterpillar
  int pos             = 0;
  HSL hsl             = {0, 0.8, 0.25};

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
