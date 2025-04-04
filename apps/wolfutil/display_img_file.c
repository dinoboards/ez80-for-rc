#include "wolfutil.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <v99x8-super.h>
#include "wait_for_key.h"

// static void apply_palette(uint8_t *surface, uint16_t width, uint16_t height) {

//   uint8_t *c = surface;

//   for (uint8_t y = 0; y < height; y++) {
//     for (uint16_t x = 0; x < width; x++) {
//       *c = gamepal[*c];
//       c++;
//     }
//   }
// }

void display_img_file() {
  // enable graphics mode and erase
  vdp_set_super_graphic_1();
  vdp_set_extended_palette(gamepal);
  vdp_cmd_vdp_to_vram(0, 0, 320, 200, 0, 0);
  vdp_cmd_wait_completion();
  printf("Erased\r\n");
  wait_for_key();
  printf("Loading file.  Please wait...\r\n");

  uint8_t *buffer = malloc(256 * 200);
  if (buffer == NULL) {
    printf("Error: Unable to allocate buffer\r\n");
    return;
  }

  FILE *f = fopen(filename, "rb");
  if (f == NULL) {
    printf("Error: Unable to open file %s\r\n", filename);
    return;
  }

  int r = fread(buffer, 1, 256 * 200, f);
  if (r != 256 * 200) {
    printf("Error: Unable to read file %s\r\n", filename);
    fclose(f);
    return;
  }
  fclose(f);

  printf("Press key to display image\r\n");
  wait_for_key();
  vdp_cmd_move_cpu_to_vram(buffer, 0, 0, 256, 200, DIX_RIGHT | DIY_DOWN, 256*200);
  vdp_cmd_wait_completion();

  free(buffer);

  wait_for_key();
}
