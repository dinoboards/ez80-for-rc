#include "colour_conversion.h"
#include <stdint.h>
#include <v99x8-super.h>

float hue2rgb(float p, float q, float t) {
  if (t < 0.0)
    t += 1.0;

  if (t > 1.0)
    t -= 1.0;

  if (t < 1.0 / 6.0)
    return p + (q - p) * 6.0 * t;

  if (t < 1.0 / 2.0)
    return q;

  if (t < 2.0 / 3.0)
    return p + (q - p) * (2.0 / 3.0 - t) * 6.0;

  return p;
}

uint8_t clamped_hue2rgb(float p, float q, float t) {
  float val = hue2rgb(p, q, t) * 255.0;
  if (val < 0.0)
    val = 0.0;
  else if (val > 255.0)
    val = 255.0;
  return (uint8_t)val;
}

void convertHSLtoRGB(const HSL hsl, RGB *const rgb) {
  if (hsl.s <= 0.0001) {
    rgb->red = rgb->blue = rgb->green = hsl.l; // achromatic
    return;
  }

  float q = hsl.l < 0.5 ? hsl.l * (1 + hsl.s) : hsl.l + hsl.s - hsl.l * hsl.s;
  float p = 2.0 * hsl.l - q;

  rgb->blue  = clamped_hue2rgb(p, q, hsl.h - 1.0 / 3.0);
  rgb->red   = clamped_hue2rgb(p, q, hsl.h + 1.0 / 3.0);
  rgb->green = clamped_hue2rgb(p, q, hsl.h);
}
