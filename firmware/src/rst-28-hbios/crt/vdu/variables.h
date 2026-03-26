#ifndef __VARIABLES__
#define __VARIABLES__

#include "../vdu.h"
#include <stddef.h>
#include <stdint.h>
#include <vdu-types.h>

#define VRAM_SIZE         ((uint24_t)0x100000)
#define FONT_8X8_STORED_Y (VRAM_SIZE - (8 * 256))

#define MAX_VDP_BYTES 16
typedef uint8_t data_t[MAX_VDP_BYTES];

#define scale_width  ((int24_t)1280)
#define scale_height ((int24_t)1024)

extern point_t         previous_gpos;
extern uint8_t         vdu_index;
extern uint24_t        vdu_required_length;
extern cursor_state_t  cursor_state;
extern mos_vdu_handler current_fn;
extern uint8_t         counter;
extern data_t          data;
extern point_t         current_gpos;
extern point_t         origin;
extern rectangle_t     gviewport;
extern tpoint_t        current_tpos;
extern trectangle_t    tviewport;
extern uint24_t        last_text_column;
extern uint24_t        last_text_row;
extern uint8_t         current_display_mode;
extern uint8_t         current_gfg_colour;
extern uint8_t         current_mode_colour_mask;
extern uint8_t         current_operation_mode;
extern uint8_t         current_tbg_colour;
extern uint8_t         current_tfg_colour;

#endif
