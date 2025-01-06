#ifndef __WL_DRAW__
#define __WL_DRAW__

#include <ez80.h>
#include <stddef.h>
#include <stdint.h>

#define EXPORT_OFFSET(fff, a)                                                                                                      \
  asm("_drpm_" #fff " equ " #a);                                                                                                   \
  asm(".global _drpm_" #fff);

typedef struct drawing_params {
  int16_t yd;       /* 0-1 */
  int16_t yendoffs; /* 2-3 */
  int16_t yoffs;    /* 4-5 */
  int16_t yw;       /* 6-7 */
  int16_t ywcount;  /* 8-9 */
  int16_t postx;    /* 10-11 */

  // projection variables
  uint16_t view_height; /* 12-13 */
} drawing_params_t;

EXPORT_OFFSET(yd, 0);           /* 0-1 */
EXPORT_OFFSET(yendoffs, 2);     /* 2-3 */
EXPORT_OFFSET(yoffs, 4);        /* 4-5 */
EXPORT_OFFSET(yw, 6);           /* 6-7 */
EXPORT_OFFSET(ywcount, 8);      /* 8-9 */
EXPORT_OFFSET(postx, 10);       /* 10-11 */
EXPORT_OFFSET(view_height, 12); /* 12-13 */

extern drawing_params_t drawing_params __data_on_chip;

#endif
