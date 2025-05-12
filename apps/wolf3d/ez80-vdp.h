#ifndef __EZ80_VDP__
#define __EZ80_VDP__

#include "v99x8-hdmi/v99x8-wolf3d.h"
#include <stdint.h>
#include <string.h>

#define SCREEN_WIDTH  256
#define SCREEN_HEIGHT 192

typedef uint8_t GRB_t;

#define RED_FRM_GRB(grb)   ((grb & 0x1C) >> 2)
#define GREEN_FRM_GRB(grb) ((grb & 0xE0) >> 5)
#define BLUE_FRM_GRB(grb)  (grb & 0x03)
#define GRB(g, r, b)       (g >> 3) << 5 | (r >> 3) << 2 | (b >> 4)

typedef struct {
  uint16_t width;
  uint16_t height;
  uint24_t size;
  uint8_t  pixels[];
} pixel_surface_t;

extern GRB_t gamepal[256];

typedef void mem_alloc(void **baseptr, uint24_t size);

static inline void create_pixel_surface(pixel_surface_t *const surface, const uint16_t width, const uint16_t height) {
  const uint16_t size = width * height;
  memset(surface->pixels, 0, size);
  surface->width  = width;
  surface->height = height;
  surface->size   = size;
}

static inline void vdp_scn_init() {
  vdp_set_mode(7, 192, PAL);
  vdp_cmd_wait_completion();
  vdp_cmd_vdp_to_vram(0, 0, 256, 192, 0, 0);
  vdp_cmd_wait_completion();
}

#define vdp_scn_write_init() bool _first = true
#define vdp_scn_write_pixel(col, x, y, width, height)                                                                              \
  {                                                                                                                                \
    if (_first) {                                                                                                                  \
      vdp_cmd_wait_completion();                                                                                                   \
      vdp_cmd_move_data_to_vram(col, x, y, width, height, DIX_RIGHT | DIY_DOWN, width * height);                                   \
      _first = false;                                                                                                              \
    } else                                                                                                                         \
      vdp_cmd_send_byte(col);                                                                                                      \
  }

// vdp_buf_xxx -> commands to in-memory surface - no changes until update_screen called
// vdp_scr_xxx -> commands direct to vdp - draws immediately

static inline void vdp_scn_clear(const uint8_t color) {
  vdp_cmd_wait_completion();
  vdp_cmd_vdp_to_vram(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, gamepal[color], DIX_RIGHT | DIY_DOWN);
}

static inline void
vdp_scn_bar(const uint16_t x, const uint16_t y, const uint16_t width, const uint16_t height, const uint8_t color) {
  vdp_cmd_wait_completion();
  vdp_cmd_vdp_to_vram(x, y, width, height, gamepal[color], DIX_RIGHT | DIY_DOWN);
}

static inline void vdp_scn_h_line(const uint16_t x, const uint16_t y, const uint16_t width, const uint8_t color) {
  vdp_cmd_wait_completion();
  vdp_cmd_vdp_to_vram(x, y, width, 1, gamepal[color], DIX_RIGHT | DIY_DOWN);
}

static inline void vdp_scn_v_line(const uint16_t x, const uint16_t y, const uint16_t height, const uint8_t color) {
  vdp_cmd_wait_completion();
  vdp_cmd_vdp_to_vram(x, y, 1, height, gamepal[color], DIX_RIGHT | DIY_DOWN);
}

static inline void vdp_scn_font(
    const uint8_t *const font_data, const uint16_t x, const uint16_t y, const uint16_t width, const uint16_t height, GRB_t color) {
  color                = gamepal[color];
  const uint8_t  first = font_data[0] ? color : 0;
  const uint16_t size  = width * height;
  vdp_cmd_wait_completion();
  vdp_cmd_logical_move_data_to_vram(first, x, y, width, height, DIX_RIGHT | DIY_DOWN, size, CMD_LOGIC_TIMP);

  for (uint24_t i = 1; i < size; i++)
    vdp_cmd_send_byte(font_data[i] ? color : 0);
}

static inline void vdp_scn_vga_picture(const uint8_t *const pix_data,
                                       const uint16_t       x,
                                       const uint16_t       y,
                                       const uint16_t       width,
                                       const uint16_t       height,
                                       const uint16_t       vga_plane_width) {

  const GRB_t first = gamepal[pix_data[0]];

  const uint8_t *p1 = &pix_data[0];
  const uint8_t *p2 = &pix_data[height * (vga_plane_width / 4)];
  const uint8_t *p3 = &pix_data[height * (vga_plane_width / 4) * 2];
  const uint8_t *p4 = &pix_data[height * (vga_plane_width / 4) * 3];

  vdp_cmd_wait_completion();
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

    p1 += (vga_plane_width - width);
    p2 += (vga_plane_width - width);
    p3 += (vga_plane_width - width);
    p4 += (vga_plane_width - width);
  }
}

static inline void vdp_scn_copy_y(const uint16_t x, const uint16_t from_y, const uint16_t to_y, const uint16_t height) {
  vdp_cmd_wait_completion();
  vdp_cmd_move_vram_to_vram_y(x, from_y, to_y, height, DIX_RIGHT | DIY_DOWN);
}

extern uint8_t view_port_buffer[SCREEN_WIDTH * SCREEN_HEIGHT];

static inline void
vdp_buf_update(const uint16_t x, const uint16_t y, const uint16_t width, const uint16_t height, const uint16_t length) {
  vdp_cmd_move_cpu_to_vram_with_palette(view_port_buffer, x, y, width, height, DIX_RIGHT | DIY_DOWN, length, gamepal);
}
#endif
