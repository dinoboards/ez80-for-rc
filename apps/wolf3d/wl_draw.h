#ifndef __WL_DRAW__
#define __WL_DRAW__

#include <ez80.h>
#include <stddef.h>
#include <stdint.h>

#define EXPORT_OFFSET(fff, a)                                                                                                      \
  asm("_drpm_" #fff " equ " #a);                                                                                                   \
  asm(".global _drpm_" #fff);

typedef struct drawing_params {
  int16_t yd;       /* 0-1   local to ScalePost */
  int16_t yendoffs; /* 2-3  */
  int16_t yoffs;    /* 4-5  local to ScalePost */
  int16_t yw;       /* 6-7  local to ScalePost */
  int16_t ywcount;  /* 8-9  local to ScalePost */
  int16_t postx;    /* 10-11 local to ScalePost */

  // projection variables
  uint16_t view_height;      /* 12-13 */
  uint16_t view_width;       /* 14-15 */
  uint16_t view_half_height; /* 16-17 */

  // texture pointer
  uint8_t *postsource; /* 18-20 */

  // projection variables
  uint16_t view_height_plus_one; /* 21-22 */
  uint16_t view_half_width;      /* 23-24 */

} drawing_params_t;

EXPORT_OFFSET(yd, 0);                /* 0-1 */
EXPORT_OFFSET(yendoffs, 2);          /* 2-3 */
EXPORT_OFFSET(yoffs, 4);             /* 4-5 */
EXPORT_OFFSET(yw, 6);                /* 6-7 */
EXPORT_OFFSET(ywcount, 8);           /* 8-9 */
EXPORT_OFFSET(postx, 10);            /* 10-11 */
EXPORT_OFFSET(view_height, 12);      /* 12-13 */
EXPORT_OFFSET(view_width, 14);       /* 14-15 */
EXPORT_OFFSET(view_half_height, 16); /* 16-17 */
EXPORT_OFFSET(postsource, 18);       /* 18-20 */

extern drawing_params_t drawing_params __data_on_chip;

#endif
