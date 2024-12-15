#include <stdint.h>

typedef uint8_t SDL_Color;

#define RGB(r, g, b) (g >> 3) << 5 | (r >> 3) << 2 | (b >> 4)

SDL_Color gamepal[] /*__data_on_chip*/ = {
#include "wolfpal.inc"
};
