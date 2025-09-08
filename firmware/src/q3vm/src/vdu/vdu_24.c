#include <vm-shared-mem.h>

/*VDU 24,x1;y1;x2;y2
VDU 24, 100;100;500;500;
VDU 24 defines a graphics viewport. The four parameters define the left, bottom,
right and top boundaries respectively, relative to the current graphics origin.
*/
void vdu_set_gviewport(void) {
  gviewport.left   = *((uint16_t *)&data[0]);
  gviewport.bottom = *((uint16_t *)&data[2]);
  gviewport.right  = *((uint16_t *)&data[4]);
  gviewport.top    = *((uint16_t *)&data[6]);

  if (gviewport.left > scale_width - 1)
    gviewport.left = scale_width - 1;

  if (gviewport.right > scale_width - 1)
    gviewport.right = scale_width - 1;

  if (gviewport.top > scale_height - 1)
    gviewport.top = scale_height - 1;

  if (gviewport.bottom > scale_height - 1)
    gviewport.bottom = scale_height - 1;

  gsviewport.left   = convert_x(gviewport.left);
  gsviewport.top    = convert_y(gviewport.top);
  gsviewport.right  = convert_x(gviewport.right);
  gsviewport.bottom = convert_y(gviewport.bottom);
}
