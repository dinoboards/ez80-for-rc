#ifndef __VDU_H
#define __VDU_H

#include <stdint.h>
#include <v99x8.h>

typedef struct point_s {
  int16_t x;
  int16_t y;
} point_t;

extern point_t point_new(const int16_t x, const int16_t y);

typedef struct line {
  point_t a;
  point_t b;
} line_t;

line_t line_new(const point_t x, const point_t y);

typedef struct rectangle {
  int24_t left;
  int24_t bottom;
  int24_t right;
  int24_t top;
} rectangle_t;

typedef struct tpoint {
  uint24_t x;
  uint24_t y;
} tpoint_t;

typedef struct trectangle {
  uint24_t left;
  uint24_t bottom;
  uint24_t right;
  uint24_t top;
} trectangle_t;

typedef struct triangle {
  point_t vt1;
  point_t vt2;
  point_t vt3;
} triangle_t;

typedef struct text_colours {
  uint8_t fg;
  uint8_t bg;
} text_colours_t;

#define RGB_BLACK                                                                                                                  \
  { 0, 0, 0 }
#define RGB_WHITE                                                                                                                  \
  { 7, 7, 7 }
#define RGB_RED                                                                                                                    \
  { 7, 0, 0 }
#define RGB_GREEN                                                                                                                  \
  { 0, 7, 0 }
#define RGB_BLUE                                                                                                                   \
  { 0, 0, 7 }
#define RGB_YELLOW                                                                                                                 \
  { 7, 7, 0 }
#define RGB_MAGENTA                                                                                                                \
  { 7, 0, 7 }
#define RGB_CYAN                                                                                                                   \
  { 0, 7, 7 }
#define RGB_FLASHING_BLACK_WHITE                                                                                                   \
  { 3, 3, 3 }
#define RGB_FLASHING_RED_CYAN                                                                                                      \
  { 7, 3, 3 }
#define RGB_FLASHING_GREEN_MAGENTA                                                                                                 \
  { 3, 7, 3 }
#define RGB_FLASHING_YELLOW_BLUE                                                                                                   \
  { 3, 3, 7 }
#define RGB_FLASHING_BLUE_YELLOW                                                                                                   \
  { 7, 7, 3 }
#define RGB_FLASHING_MAGENTA_GREEN                                                                                                 \
  { 7, 3, 7 }
#define RGB_FLASHING_CYAN_RED                                                                                                      \
  { 3, 7, 7 }
#define RGB_FLASHING_WHITE_BLACK                                                                                                   \
  { 3, 3, 3 }

typedef void (*mos_vdu_handler)(void);

extern void vdu_bs(void);
extern void vdu_lf(void);
extern void vdu_cls(void);
extern void vdu_clg(void);
extern void vdu_colour(void);
extern void vdu_gcol(void);
extern void vdu_colour_define(void);
extern void vdu_mode(void);
extern void vdu_multi_purpose(void);
extern void vdu_set_gviewport(void);
extern void vdu_plot(void);
extern void vdu_set_tviewport(void);
extern void vdu_set_origin(void);
extern void vdu_tab(void);

#define vdu_cr()                                                                                                                   \
  { current_tpos.x = tviewport.left; }

extern void init_font_patterns(void);
void        prepare_font_pattern(uint8_t ch, uint16_t x, uint16_t y);

extern int24_t convert_x(int24_t logical_x);
extern int24_t convert_y(int24_t logical_y);
extern point_t convert_point(const point_t p);

// vdu variables

extern point_t origin;
extern RGB     default_2_colour_palette[16];
extern RGB     default_4_colour_palette[16];
extern RGB     default_16_colour_palette[16];

extern rectangle_t    gviewport;
extern rectangle_t    gsviewport;
extern point_t        current_gpos;
extern point_t        previous_gpos;
extern uint8_t        current_gfg_colour;
extern uint8_t        current_operation_mode;
extern uint8_t        current_display_mode;
extern uint8_t        sysfont[(128 - ' ') * 8]; // 96*8
extern uint8_t        font_patterns[256 * 8];
extern text_colours_t font_color[256];

extern tpoint_t     current_tpos;
extern trectangle_t tviewport;
extern uint24_t     last_text_column;
extern uint24_t     last_text_row;
extern uint8_t      current_tbg_colour;
extern uint8_t      current_tfg_colour;
extern uint8_t      current_mode_colour_mask;

#define MAX_VDP_BYTES 16
extern uint8_t  data[MAX_VDP_BYTES];
extern uint8_t  vdu_index;
extern uint24_t vdu_required_length;

extern mos_vdu_handler current_fn;

#define scale_width  ((int24_t)1280)
#define scale_height ((int24_t)1024)

extern void vdu_not_implemented(void);

#define VRAM_SIZE         0x100000
#define FONT_8X8_STORED_Y (VRAM_SIZE - (8 * 256))

#endif
