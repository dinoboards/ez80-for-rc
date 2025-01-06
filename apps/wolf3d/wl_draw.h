#ifndef __WL_DRAW__
#define __WL_DRAW__

#include <ez80.h>
#include <stdint.h>

typedef struct drawing_params {
  int16_t yd;       /* 0-1 */
  int16_t yendoffs; /* 2-3 */
  int16_t yoffs;    /* 4-5 */
  int16_t yw;       /* 6-7 */
  int16_t ywcount;  /* 8-9 */

} drawing_params_t;

extern drawing_params_t drawing_params __data_on_chip;

#endif
