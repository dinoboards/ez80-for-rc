#include <stdint.h>
#include <v99x8.h>

// typedef uint8_t SDL_Color;

#define _RGB(r, g, b)                                                                                                              \
  (RGB) { ((r << 2) + 3), ((g << 2) + 3), ((b << 2) + 3) }

RGB gamepal[256] /*__data_on_chip*/ = {
#include "wolfpal.inc"
};
