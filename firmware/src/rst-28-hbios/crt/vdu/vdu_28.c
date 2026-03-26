#include "../rst-28-vars.h"

// VDU 28,lx,by,rx,ty
// VDU 28 defines a text viewport. The parameters specify the boundary of the
// viewport; the left-most column, the bottom row, the right-most column and the top
// row respectively.
void vdu_set_tviewport(void) {
  vdu_vars_t *const vdu = &hbios_vars->vdu;

  vdu->tviewport.left   = (uint24_t)vdu->data[0] * 8;
  vdu->tviewport.bottom = (uint24_t)vdu->data[1] * 8;
  vdu->tviewport.right  = (uint24_t)vdu->data[2] * 8;
  vdu->tviewport.top    = (uint24_t)vdu->data[3] * 8;

  if (vdu->tviewport.left > vdu->last_text_column)
    vdu->tviewport.left = vdu->last_text_column;

  if (vdu->tviewport.right > vdu->last_text_column)
    vdu->tviewport.right = vdu->last_text_column;

  if (vdu->tviewport.top > vdu->last_text_row)
    vdu->tviewport.top = vdu->last_text_row;

  if (vdu->tviewport.bottom > vdu->last_text_row)
    vdu->tviewport.bottom = vdu->last_text_row;

  vdu->current_tpos.x = vdu->tviewport.left;
  vdu->current_tpos.y = vdu->tviewport.top;
}
