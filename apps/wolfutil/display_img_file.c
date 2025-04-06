#include "wait_for_key.h"
#include "wolfutil.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <v99x8.h>

static void apply_palette(uint8_t *surface, uint16_t width, uint16_t height) {

  uint8_t *c = surface;

  for (uint8_t y = 0; y < height; y++) {
    for (uint16_t x = 0; x < width; x++) {
      *c = gamepal[*c];
      c++;
    }
  }
}

void display_img_file() {
  // enable graphics mode and erase
  vdp_set_mode(7, 212, PAL);
  vdp_cmd_vdp_to_vram(0, 0, 256, 212, 0, 0);

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

  vdp_set_mode(7, 212, PAL);
  erase_page_0();

  apply_palette(buffer, 256, 200);

  vdp_cpu_to_vram0(buffer, 256 * 200);

  free(buffer);
}
