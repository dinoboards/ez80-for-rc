#include "vdu.h"

/*VDU 24,x1;y1;x2;y2
VDU 24 defines a graphics viewport. The four parameters define the left, bottom,
right and top boundaries respectively, relative to the current graphics origin.
*/
void vdu_set_gviewport() {
  uint8_t *p = (uint8_t *)&gviewport;
  *p++       = data[0];
  *p++       = data[1];
  *p++       = data[2];
  *p++       = data[3];
  *p++       = data[4];
  *p++       = data[5];
  *p++       = data[6];
  *p++       = data[7];

  if (gviewport.left < 0)
    gviewport.left = 0;
  if (gviewport.left > scale_width - 1)
    gviewport.left = scale_width - 1;

  if (gviewport.right < 0)
    gviewport.right = 0;
  if (gviewport.right > scale_width - 1)
    gviewport.right = scale_width - 1;

  if (gviewport.top < 0)
    gviewport.top = 0;
  if (gviewport.top > scale_height - 1)
    gviewport.top = scale_height - 1;

  if (gviewport.bottom < 0)
    gviewport.bottom = 0;
  if (gviewport.bottom > scale_height - 1)
    gviewport.bottom = scale_height - 1;

  gsviewport.left   = convert_x(gviewport.left);
  gsviewport.top    = convert_y(gviewport.top);
  gsviewport.right  = convert_x(gviewport.right);
  gsviewport.bottom = convert_y(gviewport.bottom);
}
