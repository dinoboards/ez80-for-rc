#ifndef __VM_SHARED_MEM__
#define __VM_SHARED_MEM__

#include "vdu-types.h"
#include <stdint.h>

typedef struct shared_mem_s {
  trectangle_t tviewport;
  uint8_t      current_tbg_colour;
  tpoint_t     current_tpos;

  uint8_t _vdp_current_mode;

} shared_mem_t;

extern shared_mem_t sm;

#endif
