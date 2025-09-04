#include "../vdu.h"
#include <stddef.h>
#include <stdint.h>

point_t origin = {0, 0};

rectangle_t gviewport = {0, 0, scale_width - 1, scale_height - 1}; // inclusive or exclusive????
rectangle_t gsviewport;
point_t     current_gpos         = {0, 0};
point_t     previous_gpos        = {0, 0};
uint8_t     current_display_mode = 255;
uint24_t    last_text_column     = 63 * 8;
uint24_t    last_text_row        = 26 * 8;
uint8_t     vdu_index            = 0;
uint24_t    vdu_required_length  = 0;

mos_vdu_handler current_fn = NULL;

uint8_t counter = 8;
