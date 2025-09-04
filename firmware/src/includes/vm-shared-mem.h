#ifndef __VM_SHARED_MEM__
#define __VM_SHARED_MEM__

#include "vdu-types.h"
#include <stdint.h>

#define MAX_VDP_BYTES 16

#define data                     (sm._data)
#define current_tfg_colour       (sm._current_tfg_colour)
#define current_mode_colour_mask (sm._current_mode_colour_mask)

typedef struct shared_mem_s {
  trectangle_t tviewport;
  uint8_t      current_tbg_colour;
  tpoint_t     current_tpos;
  uint8_t      _data[MAX_VDP_BYTES];
  uint8_t      _current_tfg_colour;
  uint8_t      _current_mode_colour_mask;

  uint8_t _vdp_current_mode;

} shared_mem_t;

extern shared_mem_t sm;

#endif
