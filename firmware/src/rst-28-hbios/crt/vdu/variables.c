#include "variables.h"
#include "../vdu.h"
#include <stddef.h>
#include <stdint.h>

// point_t origin = {0, 0};

point_t        previous_gpos       = {0, 0};
uint8_t        vdu_index           = 0;
uint24_t       vdu_required_length = 0;
cursor_state_t cursor_state        = {0};

mos_vdu_handler current_fn = NULL;

uint8_t counter = 8;

data_t       data;
point_t      current_gpos;
point_t      origin;
rectangle_t  gviewport;
tpoint_t     current_tpos;
trectangle_t tviewport;
uint24_t     last_text_column;
uint24_t     last_text_row;
uint8_t      current_display_mode;
uint8_t      current_gfg_colour;
uint8_t      current_mode_colour_mask;
uint8_t      current_operation_mode;
uint8_t      current_tbg_colour;
uint8_t      current_tfg_colour;
