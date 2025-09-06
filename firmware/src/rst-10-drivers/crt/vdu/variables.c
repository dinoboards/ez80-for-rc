#include "../vdu.h"
#include <stddef.h>
#include <stdint.h>

point_t origin = {0, 0};

point_t        current_gpos        = {0, 0};
point_t        previous_gpos       = {0, 0};
uint8_t        vdu_index           = 0;
uint24_t       vdu_required_length = 0;
cursor_state_t cursor_state        = {0};

mos_vdu_handler current_fn = NULL;

uint8_t counter = 8;
