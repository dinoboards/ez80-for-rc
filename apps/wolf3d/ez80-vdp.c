#include "ez80-vdp.h"
#include "id_mm.h"
#include <string.h>

#define malloc MM_GetPtr

pixel_surface_t *vdp_create_vdp_surface(const uint16_t width, const uint16_t height) {

  uint24_t         size = sizeof(pixel_surface_t) + width * height;
  pixel_surface_t *surface;
  malloc((memptr *)&surface, size);
  memset(surface->pixels, 0, width * height);
  surface->width  = width;
  surface->height = height;
  surface->size   = width * height;
  return surface;
}

uint8_t view_port_buffer[SCREEN_WIDTH * SCREEN_HEIGHT];
