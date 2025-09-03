#include "../vdu.h"
#include <stddef.h>
#include <stdint.h>

point_t origin = {0, 0};

rectangle_t gviewport = {0, 0, scale_width - 1, scale_height - 1}; // inclusive or exclusive????
rectangle_t gsviewport;
point_t     current_gpos           = {0, 0};
point_t     previous_gpos          = {0, 0};
uint8_t     current_gfg_colour     = 0;
uint8_t     current_operation_mode = 0;
uint8_t     current_display_mode   = 255;
// uint8_t        font_patterns[256 * 8];
// text_colours_t font_color[256];

tpoint_t     current_tpos             = {0, 0};
trectangle_t tviewport                = {0, 26 * 8, 63 * 8, 0};
uint24_t     last_text_column         = 63 * 8;
uint24_t     last_text_row            = 26 * 8;
uint8_t      current_tbg_colour       = 0;
uint8_t      current_tfg_colour       = 1;
uint8_t      current_mode_colour_mask = 1;

#define MAX_VDP_BYTES 16
uint8_t  data[MAX_VDP_BYTES];
uint8_t  vdu_index           = 0;
uint24_t vdu_required_length = 0;

mos_vdu_handler current_fn = NULL;

uint8_t counter = 8;
