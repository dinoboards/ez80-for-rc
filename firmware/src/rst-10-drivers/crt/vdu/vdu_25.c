#include "../vdu.h"
#include <stdint.h>
#include <v99x8.h>
#include <vdu-functions.h>

extern bool line_clip(line_t *l);
extern void fill_triangle(const point_t *vt1, const point_t *vt2, const point_t *vt3);

// VDU: 25 (5 bytes)

/*
modes:
0	Move relative to the last point.
1	Draw a line, in the current graphics foreground colour, relative to the last point.
2	Draw a line, in the logical inverse colour, relative to the last point.
3	Draw a line, in the current graphics background colour, relative to the last point.
4	Move to the absolute position.
5	Draw a line, in the current graphics foreground colour, to the absolute coordinates specified by X and Y.
6	Draw a line, in the logical inverse colour, to the absolute coordinates specified by X and Y.
7	Draw a line, in the current graphics background colour, to the absolute coordinates specified by X and Y.
8-63	Enhanced line drawing modes.
64-71	Plot a single point.
72-79	Horizontal line fill to non-background.
80-87	Plot and fill a triangle.
88-95	Horizontal line fill to background right.
96-103	Plot and fill an axis-aligned rectangle.
104-111	Horizontal line fill to foreground.
112-119	Plot and fill a parallelogram.
120-127	Horizontal line fill to non-foreground right.
128-135	Flood-fill to non-background.
136-143	Flood-fill to foreground.
144-151	Draw a circle.
152-159	Plot and fill a disc.
160-167	Draw a circular arc.
168-175	Plot and fill a segment.
176-183	Plot and fill a sector.
185/189	Move a rectangular block.
187/191	Copy a rectangular block.
192-199	Draw an outline axis-aligned ellipse.
200-207  	Plot and fill a solid axis-aligned ellipse.
249/253	Swap a rectangular block.

*/

void vdu_plot(void) {

  // TODO: may be compatbibility issue - move should apply origin/conversion at time execuytion, not at time of rendering

  switch (data[0]) { // mode
  case 4: {          // MOVE
    previous_gpos  = current_gpos;
    current_gpos.x = *((int16_t *)&data[1]) + origin.x;
    current_gpos.y = *((int16_t *)&data[3]) + origin.y;
    return;
  }

  case 5: {
    previous_gpos = current_gpos;

    current_gpos.x = *((int16_t *)&data[1]) + origin.x;
    current_gpos.y = *((int16_t *)&data[3]) + origin.y;

    {
      line_t   l          = line_new(previous_gpos, current_gpos);
      uint24_t intersects = line_clip(&l);

      if (intersects) {
        uint24_t x  = convert_x(l.a.x);
        uint24_t y  = convert_y(l.a.y);
        uint24_t x2 = convert_x(l.b.x);
        uint24_t y2 = convert_y(l.b.y);

        vdp_draw_line(x, y, x2, y2, current_gfg_colour, current_operation_mode);
      }
      return;
    }
  }

  case 69: {
    previous_gpos = current_gpos;

    {
      current_gpos.x = *((int16_t *)&data[1]) + origin.x;
      current_gpos.y = *((int16_t *)&data[3]) + origin.y;

      vdp_cmd_wait_completion();
      vdp_cmd_pset(convert_x(current_gpos.x), convert_y(current_gpos.y), current_gfg_colour, current_operation_mode);
      return;
    }
  }

  case 85: { // triangle
    const point_t p1 = convert_point(previous_gpos);
    const point_t p2 = convert_point(current_gpos);
    previous_gpos    = current_gpos;

    current_gpos.x = *((int16_t *)&data[1]) + origin.x;
    current_gpos.y = *((int16_t *)&data[3]) + origin.y;

    {
      const point_t p3 = convert_point(current_gpos);

      fill_triangle(&p1, &p2, &p3);
      return;
    }
  }

  default:
    vdu_not_implemented();
  }
}
