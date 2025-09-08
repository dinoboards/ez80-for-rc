#ifndef __VDU_H
#define __VDU_H

#include "v99x8.h"
#include <stdint.h>
#include <vdu-types.h>
#include <vm-shared-mem.h>

extern point_t point_new(const int16_t x, const int16_t y);
extern line_t  line_new(const point_t x, const point_t y);

extern void vdu_bs(void);
extern void vdu_lf(void);
extern void vdu_cr(void);
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

void vdu_set_default_palette_2();
void vdu_set_default_palette_4();
void vdu_set_default_palette_16();

extern void init_font_patterns(void);
void        prepare_font_pattern(uint8_t ch, uint16_t x, uint16_t y);

// vdu variables

typedef struct cursor_state_s {
  uint8_t enabled : 1;
  uint8_t toggle : 1;
  uint8_t hide : 1; /* disable while moving x,y */
} cursor_state_t;

// extern point_t origin;

extern point_t        previous_gpos;
extern uint8_t        sysfont[(128 - ' ') * 8]; // 96*8
extern uint8_t        font_patterns[256 * 8];
extern text_colours_t font_color[256];
extern cursor_state_t cursor_state;

extern uint8_t  vdu_index;
extern uint24_t vdu_required_length;

extern mos_vdu_handler current_fn;

extern void vdu_not_implemented(void);

void vdu_cursor_disable();
void vdu_cursor_enable();

#define vdu_bs()                                                                                                                   \
  {                                                                                                                                \
    if (current_tpos.x > tviewport.left)                                                                                           \
      current_tpos.x -= 8;                                                                                                         \
  }

#define vdu_cr()                                                                                                                   \
  { current_tpos.x = tviewport.left; }

#endif
