#ifndef __COLOUR_CONVERSION
#define __COLOUR_CONVERSION

#include <v99x8-super.h>

typedef struct _HSL {
  float h;
  float s;
  float l;
} HSL;

extern void convertHSLtoRGB(const HSL hsl, RGB *const rgb);

#endif
