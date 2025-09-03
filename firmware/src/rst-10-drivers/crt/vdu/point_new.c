#include "../vdu.h"

point_t point_new(const int16_t x, const int16_t y) {
  point_t t;
  t.x = x;
  t.y = y;

  return t;
}
