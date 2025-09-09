#include <vdu-functions.h>
#include <vm-shared-mem.h>

/*VDU 24,x1;y1;x2;y2
VDU 24, 100;100;500;500;
VDU 24 defines a graphics viewport. The four parameters define the left, bottom,
right and top boundaries respectively, relative to the current graphics origin.
region is relative to origin
if point is outside, then  region is reset
*/
void vdu_set_gviewport(void) {
  gviewport.left   = *((uint16_t *)&data[0]) + origin.x;
  gviewport.bottom = *((uint16_t *)&data[2]) + origin.y;
  gviewport.right  = *((uint16_t *)&data[4]) + origin.x;
  gviewport.top    = *((uint16_t *)&data[6]) + origin.y;

  if (gviewport.left < 0 || gviewport.left > scale_width - 1 || gviewport.right < 0 || gviewport.right > scale_width - 1 ||
      gviewport.top < 0 || gviewport.top > scale_height - 1 || gviewport.bottom < 0 || gviewport.bottom > scale_height - 1) {

    gviewport.left   = 0;
    gviewport.top    = 0;
    gviewport.right  = 1280;
    gviewport.bottom = 1024;
  }
}
