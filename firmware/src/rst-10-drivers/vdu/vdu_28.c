#include "vdu.h"

// VDU 28,lx,by,rx,ty
// VDU 28 defines a text viewport. The parameters specify the boundary of the
// viewport; the left-most column, the bottom row, the right-most column and the top
// row respectively.
void vdu_set_tviewport() {
  uint8_t *p = (uint8_t *)&tviewport;
  *p++       = data[0];
  *p++       = data[1];
  *p++       = data[2];
  *p++       = data[3];

  if (tviewport.left < 0)
    tviewport.left = 0;
  if (tviewport.left > last_text_column)
    tviewport.left = last_text_column;

  if (tviewport.right < 0)
    tviewport.right = 0;
  if (tviewport.right > last_text_column)
    tviewport.right = last_text_column;

  if (tviewport.top < 0)
    tviewport.top = 0;
  if (tviewport.top > last_text_row)
    tviewport.top = last_text_row;

  if (tviewport.bottom < 0)
    tviewport.bottom = 0;
  if (tviewport.bottom > last_text_row)
    tviewport.bottom = last_text_row;

  current_tpos.x = tviewport.left;
  current_tpos.y = tviewport.top;
}
