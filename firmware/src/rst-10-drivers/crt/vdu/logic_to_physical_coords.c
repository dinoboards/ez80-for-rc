#include "../vdu.h"
#include <v99x8.h>

int24_t convert_x(int24_t logical_x) { return vdp_get_screen_width() * (logical_x) / scale_width; }

int24_t convert_y(int24_t logical_y) { return (vdp_get_screen_height() * ((scale_height - 1) - (logical_y))) / scale_height; }

point_t convert_point(const point_t p) { return point_new(convert_x(p.x), convert_y(p.y)); }
