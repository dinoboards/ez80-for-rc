#ifndef __VDU_H
#define __VDU_H

#include <stdint.h>
#include <v99x8.h>

typedef struct point_s {
  int16_t x;
  int16_t y;
} point_t;

extern point_t point(const int16_t x, const int16_t y);

typedef struct line {
  point_t a;
  point_t b;
} line_t;

typedef struct rectangle {
  int16_t left;
  int16_t bottom;
  int16_t right;
  int16_t top;
} rectangle_t;

typedef struct tpoint {
  uint8_t x;
  uint8_t y;
} tpoint_t;

typedef struct trectangle {
  uint8_t left;
  uint8_t bottom;
  uint8_t right;
  uint8_t top;
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
extern void vdu_cr(void);
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

extern void init_font_patterns(void);
void        prepare_font_pattern(uint8_t ch, uint16_t x, uint16_t y);

extern int16_t convert_x(int16_t logical_x);
extern int16_t convert_y(int16_t logical_y);
extern point_t convert_point(const point_t p);

// vdu variables

extern point_t origin;
extern RGB     default_2_colour_palette[16];
extern RGB     default_4_colour_palette[16];
extern RGB     default_16_colour_palette[16];

extern const int16_t scale_width;
extern const int16_t scale_height;

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
extern uint8_t      last_text_column;
extern uint8_t      last_text_row;
extern uint8_t      current_tbg_colour;
extern uint8_t      current_tfg_colour;
extern uint8_t      current_mode_colour_mask;

#define MAX_VDP_BYTES 16
extern uint8_t data[MAX_VDP_BYTES];
extern uint8_t vdu_index;
extern uint8_t vdu_required_length;

extern mos_vdu_handler current_fn;

#define scale_width  ((int16_t)1280)
#define scale_height ((int16_t)1024)

extern void vdu_not_implemented(void);

#endif
