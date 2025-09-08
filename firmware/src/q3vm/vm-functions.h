#ifndef __VM_FUNCTIONS__
#define __VM_FUNCTIONS__

#include "bytecode.h"
#include "host/vm.h"

extern vm_t vm;

extern uint24_t vm_call_void(const uint24_t pc);
// extern void vm_call_1(const uint24_t pc, uint24_t arg);
// extern void vm_call_2(const uint24_t pc, uint24_t arg, uint24_t arg2);

#define vm_vdu_cls()                                                                                                               \
  { vm_call_void(Q3VM_FN_VDU_CLS); }

#define vm_vdp_set_super_graphic_1()                                                                                               \
  { vm_call_void(Q3VM_FN_VDP_SET_SUPER_GRAPHIC_1); }

#define vm_vdu_bs()                                                                                                                \
  { vm_call_void(Q3VM_FN_VDU_BS); }

#define vm_vdu_clg()                                                                                                               \
  { vm_call_void(Q3VM_FN_VDU_CLG); }

extern void vm_vdu_colour(void);
extern void vm_vdu_gcol(void);
extern void vm_vdu_mode(void);
extern void vm_vdu_colour_define(void);
extern void vm_vdu_set_gviewport(void);
extern void vm_vdu_multi_purpose(void);
extern void vm_vdu_set_tviewport(void);
extern void VM_vdu_set_origin(void);

#endif
