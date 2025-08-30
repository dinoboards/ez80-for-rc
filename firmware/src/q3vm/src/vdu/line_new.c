#include "../vdu.h"

line_t line_new(const point_t x, const point_t y) { return *(line_t *)&x; /* probably very compiler dependant */ }
