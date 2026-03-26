#include "../rst-28-vars.h"
#include "vdu_consts.h"
#include <vdu-functions.h>

/*VDU 24,x1;y1;x2;y2
VDU 24, 100;100;500;500;
VDU 24 defines a graphics viewport. The four parameters define the left, bottom,
right and top boundaries respectively, relative to the current graphics origin.
region is relative to origin
if point is outside, then  region is reset
*/
void vdu_set_gviewport(void) {
  vdu_vars_t *const vdu = &hbios_vars->vdu;

  vdu->gviewport.left   = *((uint16_t *)&vdu->data[0]) + vdu->origin.x;
  vdu->gviewport.bottom = *((uint16_t *)&vdu->data[2]) + vdu->origin.y;
  vdu->gviewport.right  = *((uint16_t *)&vdu->data[4]) + vdu->origin.x;
  vdu->gviewport.top    = *((uint16_t *)&vdu->data[6]) + vdu->origin.y;

  if (vdu->gviewport.left < 0 || vdu->gviewport.left > scale_width - 1 || vdu->gviewport.right < 0 ||
      vdu->gviewport.right > scale_width - 1 || vdu->gviewport.top < 0 || vdu->gviewport.top > scale_height - 1 ||
      vdu->gviewport.bottom < 0 || vdu->gviewport.bottom > scale_height - 1) {

    vdu->gviewport.left   = 0;
    vdu->gviewport.top    = 0;
    vdu->gviewport.right  = 1280;
    vdu->gviewport.bottom = 1024;
  }
}
