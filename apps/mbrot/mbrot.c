/*
c program:
--------------------------------
 1. draws Mandelbrot set for Fc(z)=z*z +c
 using Mandelbrot algorithm ( boolean escape time )
-------------------------------
2. technique of creating ppm file is  based on the code of Claudio Rocchini
http://en.wikipedia.org/wiki/Image:Color_complex_plot.jpg
create 24 bit color graphic file ,  portable pixmap file = PPM
see http://en.wikipedia.org/wiki/Portable_pixmap
to see the file use external application ( graphic viewer)
 */
#include "../common/config_request.h"
#include <cpm.h>
#include <math.h>
#include <stdio.h>
#include <v99x8.h>

RGB palette[16] = {
    {0, 0, 0}, {1, 0, 0}, {2, 0, 0},  {3, 0, 0},  {4, 0, 0},  {5, 0, 0},  {6, 0, 0},  {7, 0, 0},
    {8, 0, 0}, {9, 0, 0}, {10, 0, 0}, {11, 0, 0}, {12, 0, 0}, {13, 0, 0}, {14, 0, 0}, {15, 0, 0},
};

#define ITERATION_MAX 16
#define CX_MIN        -2.5
#define CX_MAX        1.5
#define CY_MIN        -2.0
#define CY_MAX        2.0
#define IX_MAX        512

#define PIXEL_WIDTH ((CX_MAX - CX_MIN) / IX_MAX)

#define ESCAPE_RADIUS 2
/* bail-out value , radius of circle ;  */
#define ER2 (ESCAPE_RADIUS * ESCAPE_RADIUS)

float Cx;
float Cy;
float Zx;
float Zy;
float Zx2;
float Zy2;

/* screen ( integer) coordinate */
uint24_t iX, iY;
uint8_t  iteration;

int main(void) {
  const uint8_t refresh_rate = getVideoMode();
  const uint8_t lines        = getLineCount();

  vdp_set_mode(6, lines, refresh_rate);
  vdp_set_palette(palette);
  vdp_erase_bank0(0);

  printf("Press any key to abort\r\n");

  const float pixelHeight = ((CY_MAX - CY_MIN) / lines);

  for (iY = 0; iY < lines; iY++) {
    Cy = CY_MIN + iY * pixelHeight;

    if (fabs(Cy) < pixelHeight / 2)
      Cy = 0.0;

    for (iX = 0; iX < IX_MAX; iX++) {
      Cx = CX_MIN + iX * PIXEL_WIDTH;

      Zx  = 0.0;
      Zy  = 0.0;
      Zx2 = Zx * Zx;
      Zy2 = Zy * Zy;

      for (iteration = 0; iteration < ITERATION_MAX && ((Zx2 + Zy2) < ER2); iteration++) {
        Zy  = 2 * Zx * Zy + Cy;
        Zx  = Zx2 - Zy2 + Cx;
        Zx2 = Zx * Zx;
        Zy2 = Zy * Zy;
      };

      if (((iX & 15) == 0) && cpm_rawio() != 0)
        return 0;

      pointSet(iX, iY, iteration, CMD_LOGIC_IMP);
    }
  }

  return 0;
}
