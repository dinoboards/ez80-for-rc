#ifndef __VARIABLES__
#define __VARIABLES__

#include <stdint.h>

#define VRAM_SIZE         ((uint24_t)0x100000)
#define FONT_8X8_STORED_Y (VRAM_SIZE - (8 * 256))

#define scale_width  ((int24_t)1280)
#define scale_height ((int24_t)1024)

#endif
