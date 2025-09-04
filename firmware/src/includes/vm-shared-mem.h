#ifndef __VM_SHARED_MEM__
#define __VM_SHARED_MEM__

#include "vdu-types.h"
#include <stdint.h>

#define MAX_VDP_BYTES 16
typedef uint8_t data_t[MAX_VDP_BYTES];

#define current_display_mode     (*((uint8_t *)&sm._current_display_mode))
#define current_gfg_colour       (*((uint8_t *)&sm._current_gfg_colour))
#define current_mode_colour_mask (*((uint8_t *)&sm._current_mode_colour_mask))
#define current_operation_mode   (*((uint8_t *)&sm._current_operation_mode))
#define current_tbg_colour       (*((uint8_t *)&sm._current_tbg_colour))
#define current_tfg_colour       (*((uint8_t *)&sm._current_tfg_colour))
#define current_tpos             (*((tpoint_t *)&sm._current_tpos))
#define data                     (*((data_t *)&sm._data))
#define gsviewport               (*((trectangle_t *)&sm._gsviewport))
#define gviewport                (*((trectangle_t *)&sm._gviewport))
#define last_text_column         (*((uint24_t *)&sm._last_text_column))
#define last_text_row            (*((uint24_t *)&sm._last_text_row))
#define tviewport                (*((trectangle_t *)&sm._tviewport))

typedef struct shared_mem_s {
  data_t       _data;
  rectangle_t  _gsviewport;
  rectangle_t  _gviewport;
  tpoint_t     _current_tpos;
  trectangle_t _tviewport;
  uint24_t     _last_text_column;
  uint24_t     _last_text_row;
  uint8_t      _current_display_mode;
  uint8_t      _current_gfg_colour;
  uint8_t      _current_mode_colour_mask;
  uint8_t      _current_operation_mode;
  uint8_t      _current_tbg_colour;
  uint8_t      _current_tfg_colour;

  uint8_t _vdp_current_mode;

} shared_mem_t;

extern shared_mem_t sm;

#endif
