#ifndef __VDU_POINT_CONVERSION__
#define __VDU_POINT_CONVERSION__

#include "vdu-types.h"
#include <stdint.h>

extern int24_t convert_x(int24_t logical_x);
extern int24_t convert_y(int24_t logical_y);
extern point_t convert_point(const point_t p);
extern void    vdu_cls(void);
extern void    vdu_cursor_disable(void);
extern void    vdu_cursor_enable(void);
extern void    vdu_set_default_palette_2(void);
extern void    vdu_set_default_palette_4(void);
extern void    vdu_set_default_palette_16(void);

#endif
