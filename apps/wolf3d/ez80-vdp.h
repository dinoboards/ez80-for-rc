#ifndef __EZ80_VDP__
#define __EZ80_VDP__

#include "v99x8-hdmi/v99x8-wolf3d.h"
#include <stdint.h>
#include <string.h>

typedef struct {
  uint16_t width;
  uint16_t height;
  uint24_t size;
  uint8_t  pixels[];
} pixel_surface_t;

extern pixel_surface_t *vdp_create_vdp_surface(uint16_t width, uint16_t height);

extern pixel_surface_t *_screenBuffer;

extern uint8_t gamepal[256];

// vdp_buf_xxx -> commands to in-memory surface - no changes until update_screen called
// vdp_scr_xxx -> commands direct to vdp - draws immediately

static inline void vdp_buf_clear(uint8_t colour) {
  // TODO memset screenBuffer
  memset(_screenBuffer->pixels, colour, _screenBuffer->size);
}

static inline void
vdp_scn_bar(const uint16_t x, const uint16_t y, const uint16_t width, const uint16_t height, const uint8_t color) {
  vdp_cmd_vdp_to_vram(x, y, width, height, gamepal[color], DIX_RIGHT | DIY_DOWN);
}

static inline void vdp_scn_font(const uint8_t *const font_data,
                                const uint16_t       x,
                                const uint16_t       y,
                                const uint16_t       width,
                                const uint16_t       height,
                                uint8_t              color) {
  color                = gamepal[color];
  const uint8_t  first = font_data[0] ? color : 0;
  const uint16_t size  = width * height;
  vdp_cmd_logical_move_data_to_vram(first, x, y, width, height, DIX_RIGHT | DIY_DOWN, width * height, CMD_LOGIC_TIMP);

  for (uint24_t i = 1; i < size; i++)
    vdp_cmd_send_byte(font_data[i] ? color : 0);
}

static inline void vdp_scn_vga_picture(
    const uint8_t *const pix_data, const uint16_t x, const uint16_t y, const uint16_t width, const uint16_t height) {

  const uint8_t first = gamepal[pix_data[0]];

  const uint8_t *p1 = &pix_data[0];
  const uint8_t *p2 = &pix_data[height * (width / 4)];
  const uint8_t *p3 = &pix_data[height * (width / 4) * 2];
  const uint8_t *p4 = &pix_data[height * (width / 4) * 3];

  vdp_cmd_move_data_to_vram(first, x, y, width, height, DIX_RIGHT | DIY_DOWN, width * height);

  bool skip_first = true;
  for (int ysrc = 0; ysrc < height; ysrc++) {
    for (int xsrc = 0; xsrc < width; xsrc += 4) {
      if (skip_first) {
        skip_first = false;
        p1++;
        vdp_cmd_send_byte(gamepal[*p2++]);
        vdp_cmd_send_byte(gamepal[*p3++]);
        vdp_cmd_send_byte(gamepal[*p4++]);
        continue;
      }

      vdp_cmd_send_byte(gamepal[*p1++]);
      vdp_cmd_send_byte(gamepal[*p2++]);
      vdp_cmd_send_byte(gamepal[*p3++]);
      vdp_cmd_send_byte(gamepal[*p4++]);
    }
  }
}

#endif
