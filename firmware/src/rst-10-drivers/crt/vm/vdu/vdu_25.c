#include "../vdu.h"
#include "vdu_25/line_clip.c"
#include "vdu_25/triangle.c"
#include <v99x8.h>

#include "bg_lib.h"
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

void vdu_plot() {

  switch (data[0]) { // mode
  case 85: {         // triangle

    printf("plot 85, %d, %d\n", data[1], data[2], data[3], data[4]);

    // const point_t p1 = convert_point(previous_gpos);
    // const point_t p2 = convert_point(current_gpos);
    // previous_gpos    = current_gpos;

    // {
    //   uint8_t *const bptr_x = (uint8_t *)&current_gpos.x;
    //   bptr_x[0]             = data[1];
    //   bptr_x[1]             = data[2];

    //   {
    //     uint8_t *const bptr_y = (uint8_t *)&current_gpos.y;
    //     bptr_y[0]             = data[3];
    //     bptr_y[1]             = data[4];

    //     {
    //       const point_t p3 = convert_point(current_gpos);

    //       // printf("fill_triangle((%d, %d), (%d, %d), (%d, %d));", p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);
    //       // fill_triangle(&p1, &p2, &p3);
    //       // draw line from p1 to p2
    //       // then to p2, then back to p1

    //       return;
    //     }
    //   }
    // }
  }

  default:
    vdu_not_implemented();
  }
}
