#include "wolfutil.h"
#include <stdio.h>
#include <stdlib.h>
#include <v99x8.h>

const uint16_t src_width  = 320;
const uint16_t dst_width  = 256;
const uint16_t crop_right = 32 - 10;
const uint16_t crop_left  = 10;
const uint16_t crop       = crop_right + crop_left;
const uint16_t skip_every = dst_width / (64 - crop);

void resize_to_256_200(uint8_t *surface) {

  // reduce the horizontal resolution of the image from 320x200 to 256x200

  const uint8_t *c    = surface;
  uint8_t       *dest = surface;

  uint16_t dst_x = 0;
  for (uint8_t y = 0; y < 200; y++) {
    for (uint16_t x = 0; x < 320; x++) {
      if (x >= crop_left && x < (src_width - crop_right)) {
        if (dst_x == skip_every) {
          dst_x = 0;
        } else {
          *dest++ = *c;
          dst_x++;
        }
      }
      c++;
    }
  }
}

void create_signon_image(void) {
  resize_to_256_200(signon);

  filename = "C:SIGNON.IMG";

  FILE *f = fopen(filename, "wb");
  fwrite(signon, 1, 256 * 200, f);
  fclose(f);

  printf("SIGNON.IMG image created\r\n");

  display_img_file();
}
