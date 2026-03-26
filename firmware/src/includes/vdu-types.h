#ifndef __VDU_TYPES__
#define __VDU_TYPES__

#include <stdint.h>

typedef struct point_s {
  int16_t x;
  int16_t y;
} point_t; /* 4 BYTES */

extern point_t point_new(const int16_t x, const int16_t y);

typedef struct line {
  point_t a;
  point_t b;
} line_t;

line_t line_new(const point_t x, const point_t y);

typedef struct rectangle {
  int24_t left;
  int24_t bottom;
  int24_t right;
  int24_t top;
} rectangle_t; /* 12 BYTES */

typedef struct tpoint {
  uint24_t x;
  uint24_t y;
} tpoint_t; /* 6 BYTES */

typedef struct trectangle {
  uint24_t left;
  uint24_t bottom;
  uint24_t right;
  uint24_t top;
} trectangle_t; /* 12 BYTES */

typedef struct triangle {
  point_t vt1;
  point_t vt2;
  point_t vt3;
} triangle_t;

typedef struct text_colours {
  uint8_t fg;
  uint8_t bg;
} text_colours_t;

typedef void (*mos_vdu_handler)(void);

#endif
