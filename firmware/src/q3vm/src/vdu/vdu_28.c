#include <vm-shared-mem.h>

// VDU 28,lx,by,rx,ty
// VDU 28 defines a text viewport. The parameters specify the boundary of the
// viewport; the left-most column, the bottom row, the right-most column and the top
// row respectively.
void vdu_set_tviewport(void) {
  tviewport.left   = (uint24_t)data[0] * 8;
  tviewport.bottom = (uint24_t)data[1] * 8;
  tviewport.right  = (uint24_t)data[2] * 8;
  tviewport.top    = (uint24_t)data[3] * 8;

  if (tviewport.left > last_text_column)
    tviewport.left = last_text_column;

  if (tviewport.right > last_text_column)
    tviewport.right = last_text_column;

  if (tviewport.top > last_text_row)
    tviewport.top = last_text_row;

  if (tviewport.bottom > last_text_row)
    tviewport.bottom = last_text_row;

  current_tpos.x = tviewport.left;
  current_tpos.y = tviewport.top;
}
