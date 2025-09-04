#ifndef __VM_SHARED_MEM__
#define __VM_SHARED_MEM__

#include "vdu-types.h"
#include <stdint.h>

#define MAX_VDP_BYTES 16

#define current_gfg_colour       (sm._current_gfg_colour)
#define current_mode_colour_mask (sm._current_mode_colour_mask)
#define current_operation_mode   (sm._current_operation_mode)
#define current_tfg_colour       (sm._current_tfg_colour)
#define current_tpos             (sm._current_tpos)
#define data                     (sm._data)

typedef struct shared_mem_s {
  tpoint_t     _current_tpos;
  trectangle_t tviewport;
  uint8_t      _current_gfg_colour;
  uint8_t      _current_mode_colour_mask;
  uint8_t      _current_operation_mode;
  uint8_t      _current_tfg_colour;
  uint8_t      _data[MAX_VDP_BYTES];
  uint8_t      current_tbg_colour;

  uint8_t _vdp_current_mode;

} shared_mem_t;

extern shared_mem_t sm;

#endif
