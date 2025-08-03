#include "vdu.h"
#include <v99x8.h>

int16_t convert_x(int16_t logical_x) { return vdp_get_screen_width() * (logical_x + origin.x) / scale_width; }

int16_t convert_y(int16_t logical_y) { return (vdp_get_screen_height() * (scale_height - (logical_y + origin.y))) / scale_height; }

point_t convert_point(const point_t p) {
  point_t result;
  result.x = convert_x(p.x);
  result.y = convert_y(p.y);

  return result;
}
