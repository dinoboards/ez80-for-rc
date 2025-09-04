#ifndef __VM_FUNCTIONS__
#define __VM_FUNCTIONS__

#include "bytecode.h"
#include "host/vm.h"

extern vm_t vm;

extern void vm_call_void(const uint24_t pc);
extern void vm_call_1(const uint24_t pc, uint24_t arg);
extern void vm_call_2(const uint24_t pc, uint24_t arg, uint24_t arg2);

#define vm_vdp_set_super_graphic_1()                                                                                               \
  { vm_call_void(Q3VM_FN_VDP_SET_SUPER_GRAPHIC_1); }
#define vm_vdp_set_super_graphic_2()                                                                                               \
  { vm_call_void(Q3VM_FN_VDP_SET_SUPER_GRAPHIC_2); }
#define vm_vdp_set_graphic_4()                                                                                                     \
  { vm_call_void(Q3VM_FN_VDP_SET_GRAPHIC_4); }
#define vm_vdp_set_super_graphic_21()                                                                                              \
  { vm_call_void(Q3VM_FN_VDP_SET_SUPER_GRAPHIC_21); }
#define vm_vdp_set_super_graphic_26()                                                                                              \
  { vm_call_void(Q3VM_FN_VDP_SET_SUPER_GRAPHIC_26); }
#define vm_vdp_set_lines(lines)                                                                                                    \
  { vm_call_1(Q3VM_FN_VDP_SET_LINES, lines); }
#define vm_vdp_set_refresh(refresh_rate)                                                                                           \
  { vm_call_1(Q3VM_FN_VDP_SET_REFRESH, refresh_rate); }
#define vm_vdu_set_default_palette_2()                                                                                             \
  { vm_call_void(Q3VM_FN_VDU_SET_DEFAULT_PALETTE_2); }
#define vm_vdu_set_default_palette_4()                                                                                             \
  { vm_call_void(Q3VM_FN_VDU_SET_DEFAULT_PALETTE_4); }
#define vm_vdu_set_default_palette_16()                                                                                            \
  { vm_call_void(Q3VM_FN_VDU_SET_DEFAULT_PALETTE_16); }

#define vm_vdu_cls()                                                                                                               \
  { vm_call_void(Q3VM_FN_VDU_CLS); }

#define vm_vdp_set_remap(back, fore)                                                                                               \
  { vm_call_2(Q3VM_FN_VDP_SET_REMAP, back, fore); }

extern void vm_vdu_colour(void);
extern void vm_vdu_gcol(void);

#endif
