#include <v99x8.h>
#include <vdu-standards-colors.h>
#include <vm-shared-mem.h>

extern const RGB default_2_colour_palette[16];
extern const RGB default_4_colour_palette[16];
extern const RGB default_16_colour_palette[16];

// VDU 19,l,p,r,g,b
// VDU 19 is used to define the physical colours associated with the logical colour l.
// If p <= 15 & p >= 0, r, g and b are ignored, and one of the standard colour settings is
// used. This is equivalent to COLOUR l,p.
// If p = 16, the palette is set up to contain the levels of red, green and blue dictated
// by r, g and b. This is equivalent to COLOUR l,r,g,b.
// If p = 24, the border is given colour components according to r, g and b.
// If p = 25, the mouse logical colour l is given colour components according to r, g
// and b. This is equivalent to MOUSE COLOUR l,r,g,b.

// TODO currently assumes in 16 colour mode -make it work for all modes
// TODO: implement 2nd form of this - apply actual rgb values
void vdu_colour_define(void) {
  const uint24_t l = data[0];
  const uint24_t p = data[1];

  if (p <= 15 && p >= 0) {
    const RGB physical_colour = default_16_colour_palette[p];

    vdp_reg_write(16, l & 15);
    vdp_out_pal((physical_colour.red & 7) * 16 + (physical_colour.blue & 7));
    vdp_out_pal(physical_colour.green & 7);
  }
}
