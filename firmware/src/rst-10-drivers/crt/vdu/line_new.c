#include "../vdu.h"

line_t line_new(const point_t x, const point_t y) {
  line_t r;
  r.a = x;;
  r.b = y;

  return r;
}
